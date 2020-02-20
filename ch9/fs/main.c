#include "type.h"
#include "config.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"
#include "hd.h"

PRIVATE void init_fs();
PRIVATE void mkfs();
PRIVATE void read_super_block(int dev);

// task_fs, sends DEV_OPEN to get disk info
PUBLIC void task_fs() {
    printl("Task FS begins.\n");
    init_fs();
    
    while (1) {
        send_recv(RECEIVE, ANY, &fs_msg);

        int src = fs_msg.source;
        pcaller = &proc_table[src];   // defined in global.h

        switch(fs_msg.type) {
            case OPEN:
                fs_msg.FD = do_open();
                break;
            case CLOSE:
                fs_msg.RETVAL = do_close();
                break;
            default:
                dump_msg("FS::unknown message:", &fs_msg);
                assert(0);
                break;
        }

        fs_msg.type = SYSCALL_RET;
        send_recv(SEND, src, &fs_msg);
    }
}

// open the HD and initialize the file system
PRIVATE void init_fs() {
    MESSAGE driver_msg;
    driver_msg.type = DEV_OPEN;
    driver_msg.DEVICE = MINOR(ROOT_DEV);
    assert(dd_map[MAJOR(ROOT_DEV)].driver_nr != INVALID_DRIVER);
    send_recv(BOTH, dd_map[MAJOR(ROOT_DEV)].driver_nr, &driver_msg);

    mkfs();
}

// see Figure 9.6 for file system design
PRIVATE void mkfs() {
    MESSAGE driver_msg;
    int i, j;

    int bits_per_sect = SECTOR_SIZE * 8;

    // get the partition info (geometry) of ROOTDEV
    struct part_info geo;
    driver_msg.type = DEV_IOCTL;
    driver_msg.DEVICE = MINOR(ROOT_DEV);
    driver_msg.REQUEST = DIOCTL_GET_GEO;
    driver_msg.BUF = &geo;
    driver_msg.PROC_NR = TASK_FS;
    assert(dd_map[MAJOR(ROOT_DEV)].driver_nr != INVALID_DRIVER);
    send_recv(BOTH, dd_map[MAJOR(ROOT_DEV)].driver_nr, &driver_msg);

    printl("dev size: 0x%x sectors\n", geo.size);

    // write super block to sector #1
    struct super_block sb;
    sb.magic = MAGIC_V1;
    sb.nr_inodes = bits_per_sect;  // 4096, each takes 1 bit
    sb.nr_inode_sects = sb.nr_inodes * INODE_SIZE / SECTOR_SIZE;
    sb.nr_sects = geo.size;
    sb.nr_imap_sects = 1;
    sb.nr_smap_sects = sb.nr_sects / bits_per_sect + 1;
    sb.n_1st_sect = 1 + 1 + sb.nr_imap_sects + sb.nr_smap_sects + sb.nr_inode_sects;  // first sector for root dir
    sb.root_inode = ROOT_INODE;
    sb.inode_size = INODE_SIZE;
    struct inode x;
    sb.inode_isize_off = (int)&x.i_size - (int)&x;
    sb.inode_start_off = (int)&x.i_start_sect - (int)&x;
    sb.dir_ent_size = DIR_ENTRY_SIZE;
    struct dir_entry de;
    sb.dir_ent_inode_off = (int)&de.inode_nr - (int)&de;
    sb.dir_ent_fname_off = (int)&de.name - (int)&de;

    memset(fsbuf, 0x90, SECTOR_SIZE);  // fill-up remaining space with 0x90
    memcpy(fsbuf, &sb, SUPER_BLOCK_SIZE);
    WR_SECT(ROOT_DEV, 1);     // write through system call

    printl("devbase:0x%x00, sb:0x%x00, imap:0x%x00, smap:0x%x00\n"
           "        inodes:0x%x00, 1st_sector:0x%x00\n",
           geo.base * 2,
           (geo.base + 1) * 2,
           (geo.base + 1 + 1) * 2,
           (geo.base + 1 + 1 + sb.nr_imap_sects) * 2,
           (geo.base + 1 + 1 + sb.nr_imap_sects + sb.nr_smap_sects) * 2,
           (geo.base + sb.n_1st_sect) * 2);

    // write inode bitmap into sector 2
    memset(fsbuf, 0, SECTOR_SIZE);
    for (i = 0; i < (NR_CONSOLES + 2); i++) {
        fsbuf[0] |= 1 << i;
    }
    assert(fsbuf[0] == 0x1F);   // 0001 1111
                                // bit 0: reserved, bit 1: for root dir '/'
                                // bit 2-4: /dev_tty0-2
    WR_SECT(ROOT_DEV, 2);

    // write sector map after inode map
    memset(fsbuf, 0, SECTOR_SIZE);
    int nr_sects = NR_DEFAULT_FILE_SECTS + 1;  // sectors for root dir + 1
    // set all used sectors to 1
    for (i = 0; i < nr_sects / 8; i++) {
        fsbuf[i] = 0xFF;
    }
    for (j = 0; j < nr_sects % 8; j++) {
        fsbuf[i] |= (1 << j);
    }
    WR_SECT(ROOT_DEV, 2 + sb.nr_imap_sects);

    // set the rest of sect-map to all 0
    memset(fsbuf, 0, SECTOR_SIZE);
    for (i = 1; i < sb.nr_smap_sects; i++) {
        WR_SECT(ROOT_DEV, 2 + sb.nr_imap_sects + i);
    }

    // setup inode array for root dir and dev_tty files
    memset(fsbuf, 0, SECTOR_SIZE);
    struct inode* pi = (struct inode*)fsbuf;
    // '/'
    pi->i_mode = I_DIRECTORY;
    pi->i_size = DIR_ENTRY_SIZE * 4; // 4 files: '/' and 3 dev_tty
    pi->i_start_sect = sb.n_1st_sect;
    pi->i_nr_sects = NR_DEFAULT_FILE_SECTS;
    // dev_tty
    for (i = 0; i < NR_CONSOLES; i++) {
        pi = (struct inode*)(fsbuf + (INODE_SIZE * (i + 1)));
        pi->i_mode = I_CHAR_SPECIAL;
        pi->i_size = 0;
        pi->i_start_sect = MAKE_DEV(DEV_CHAR_TTY, i);
        pi->i_nr_sects = 0;
    }
    WR_SECT(ROOT_DEV, 2 + sb.nr_imap_sects + sb.nr_smap_sects);

    // set the dir_entry for root dir '/'
    memset(fsbuf, 0, SECTOR_SIZE);

    struct dir_entry* pde = (struct dir_entry*)fsbuf;
    pde->inode_nr = 1;
    strcpy(pde->name, ".");
    // setup inode_nr for dev_tty files
    for (i = 0; i < NR_CONSOLES; i++) {
        pde++;
        pde->inode_nr = i + 2;    // dev_tty0 starts from 2
        sprintf(pde->name, "dev_tty%d", i);
    }
    WR_SECT(ROOT_DEV, sb.n_1st_sect);
}

// R/W into a provided sector of a HD device
PUBLIC int rw_sector(int io_type, int dev, u64 pos, int bytes, int proc_nr, void* buf) {
    MESSAGE driver_msg;

    driver_msg.type = io_type;
    driver_msg.DEVICE = MINOR(dev);
    driver_msg.POSITION = pos;
    driver_msg.BUF = buf;
    driver_msg.CNT = bytes;
    driver_msg.PROC_NR = proc_nr;
    assert(dd_map[MAJOR(dev)].driver_nr != INVALID_DRIVER);
    send_recv(BOTH, dd_map[MAJOR(dev)].driver_nr, &driver_msg);

    return 0;
}

// read super block from dev and write into a super_block data structure
PRIVATE void read_super_block(int dev) {

}

// get the super block from super_block array, according to given dev
PUBLIC struct super_block* get_super_block(int dev) {
    struct super_block* sb = super_block;   // defined in global.h
    for (; sb < &super_block[NR_SUPER_BLOCK]; sb++) {
        if (sb->sb_dev == dev) {
            return sb;
        }
    }

    panic("super block of device %d is not found. \n", dev);

    return 0;
}

// get the inode pointer of given inode num
PUBLIC struct inode* get_inode(int dev, int num) {

}

// decrease the reference number (i_cnt) of a given inode,
// when i_cnt reaches 0, put it from the cache inode_table[] back to disk
PUBLIC void put_inode(struct inode* pinode) {
    assert(pinode->i_cnt > 0);
    pinode->i_cnt--;
}

// write the inode back to disk
PUBLIC void sync_inode(struct inode* p) {
    struct inode* pinode;
    struct super_block* sb = get_super_block(p->i_dev);
    int blk_nr = 1 + 1 + sb->nr_imap_sects + sb->nr_smap_sects +
                 ((p->i_num - 1) / (SECTOR_SIZE / INODE_SIZE));
    RD_SECT(p->i_dev, blk_nr);    // get the sector into fsbuf
    pinode = (struct inode*)((u8*)fsbuf + 
                              (((p->i_num - 1) % (SECTOR_SIZE / INODE_SIZE)) * INODE_SIZE));
    
    pinode->i_mode = p->i_mode;
    pinode->i_size = p->i_size;
    pinode->i_start_sect = p->i_start_sect;
    pinode->i_nr_sects = p->i_nr_sects;
    WR_SECT(p->i_dev, blk_nr);
}