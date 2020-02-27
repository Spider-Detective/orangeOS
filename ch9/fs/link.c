#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"
#include "hd.h"

// remove a file, also clear the inode in inode_array[]
// return 0 if success; otherwise return -1
PUBLIC int do_unlink() {
    // read, find and validate file
    char pathname[MAX_PATH];
    int name_len = fs_msg.NAME_LEN;
    int src = fs_msg.source;
    assert(name_len < MAX_PATH);
    phys_copy((void*)va2la(TASK_FS, pathname),
              (void*)va2la(src, fs_msg.PATHNAME),
              name_len);
    pathname[name_len] = 0;

    if (strcmp(pathname, "/") == 0) {
        printl("FS::do_unlink():: cannot unlink the root\n");
        return -1;
    }

    int inode_nr = search_file(pathname);
    if (inode_nr == INVALID_INODE) {
        printl("FS::do_unlink():: file not found, search_file() returns invalid inode: %s\n", pathname);
        return -1;
    }

    char filename[MAX_PATH];
    struct inode* dir_inode;
    if (strip_path(filename, pathname, &dir_inode) != 0) {
        return -1;
    }

    struct inode* pin = get_inode(dir_inode->i_dev, inode_nr);
    if (pin->i_mode != I_REGULAR) {  // can only remove regular files
        printl("cannot remove file %s, because it is not a regular file.\n", pathname);
        return -1;
    }

    /*
     * Currently i_cnt should be 1, since we called get_inode above
     * if larger than 1, the file is opened by other process
     */ 
    if (pin->i_cnt > 1) {  
        printl("cannot remove file %s, because pin->i_cnt is %d.\n", pathname, pin->i_cnt);
        return -1;
    }

    // all checks passed, start to remove
    struct super_block* sb = get_super_block(pin->i_dev);

    // free the bit in i-map
    int byte_idx = inode_nr / 8;   // byte index in i-map
    int bit_idx = inode_nr % 8;
    assert(byte_idx < SECTOR_SIZE);
    // read sector 2 to skip bootsect and superblk
    RD_SECT(pin->i_dev, 2);
    assert(fsbuf[byte_idx % SECTOR_SIZE] & (1 << bit_idx));
    fsbuf[byte_idx % SECTOR_SIZE] &= ~(1 << bit_idx);
    WR_SECT(pin->i_dev, 2);

    // free the bits in s-map
    bit_idx = pin->i_start_sect - sb->n_1st_sect + 1;
    byte_idx = bit_idx / 8;
    int bits_left = pin->i_nr_sects;
    int byte_cnt = (bits_left - (8 - (bit_idx % 8))) / 8;

    // current sector pos
    int s = 2 + sb->nr_imap_sects + byte_idx / SECTOR_SIZE;
    RD_SECT(pin->i_dev, s);

    // clear the first byte
    int i;
    for (i = bit_idx % 8; (i < 8) && bits_left; i++, bits_left--) {
        assert((fsbuf[byte_idx % SECTOR_SIZE] >> i & 1) == 1);
        fsbuf[byte_idx % SECTOR_SIZE] &= ~(1 << i);
    }
    // clear the bytes from the second to the second to last
    int k;
    i = (byte_idx % SECTOR_SIZE) + 1;
    for (k = 0; k < byte_cnt; k++, i++, bits_left -= 8) {
        if (i == SECTOR_SIZE) {
            i = 0;
            WR_SECT(pin->i_dev, s);
            RD_SECT(pin->i_dev, ++s);
        }
        assert(fsbuf[i] == 0xFF);
        fsbuf[i] = 0;
    }
    // clear the last byte
    if (i == SECTOR_SIZE) {
        i = 0;
        WR_SECT(pin->i_dev, s);
        RD_SECT(pin->i_dev, ++s);
    }
    unsigned char mask = ~((unsigned char)(~0) << bits_left);
    assert((fsbuf[i] & mask) == mask);
    fsbuf[i] &= (~0) << bits_left;
    WR_SECT(pin->i_dev, s);

    // clear the inode in inode map
    pin->i_mode = 0;
    pin->i_size = 0;
    pin->i_start_sect = 0;
    pin->i_nr_sects = 0;
    sync_inode(pin);
    put_inode(pin);   // correspond to get_inode()

    // set the inode number to 0 in directory entry
    // we only clear the entry and leave the empty entry inside the directory, see Figure 9.16
    int dir_blk0_nr = dir_inode->i_start_sect;
    int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE) / SECTOR_SIZE;
    int nr_dir_entries = dir_inode->i_size / DIR_ENTRY_SIZE;

    int m = 0;
    struct dir_entry* pde = 0;
    int flg = 0;
    int dir_size = 0;

    for (i = 0; i< nr_dir_blks; i++) {
        RD_SECT(dir_inode->i_dev, dir_blk0_nr + i);
        pde = (struct dir_entry*)fsbuf;
        
        int j;
        for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++, pde++) {
            if (++m > nr_dir_entries) {
                break;
            }

            if (pde->inode_nr == inode_nr) {
                memset(pde, 0, DIR_ENTRY_SIZE);
                WR_SECT(dir_inode->i_dev, dir_blk0_nr + i);
                flg = 1;
                break;
            }
            if (pde->inode_nr != INVALID_INODE) {
                dir_size += DIR_ENTRY_SIZE;
            }
        }

        // if looped over all dir or file is found
        if (m > nr_dir_entries || flg) {
            break;
        }
    }
    assert(flg);
    if (m == nr_dir_entries) {  
        // if the file to delete is the last one in the dir, we recalculate the size of dir
        // to eliminate the empty dirs
        dir_inode->i_size = dir_size;
        sync_inode(dir_inode);
    }

    return 0;
}