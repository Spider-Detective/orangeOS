#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "tty.h"
#include "console.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"

// read/write to a file by using syscall
// return the number of bytes that has been read/written
PUBLIC int do_rdwt() {
    // extract the info from user call in read/write.c
    int fd = fs_msg.FD;
    void* buf = fs_msg.BUF;
    int len = fs_msg.CNT;
    int src = fs_msg.source;    // user call's proc number

    assert((pcaller->filp[fd] >= &f_desc_table[0]) &&
           (pcaller->filp[fd] < &f_desc_table[NR_FILE_DESC]));

    if (!(pcaller->filp[fd]->fd_mode & O_RDWR)) {  // check if it is ok to read/write
        return -1;
    }

    int pos = pcaller->filp[fd]->fd_pos;
    struct inode* pin = pcaller->filp[fd]->fd_inode;

    assert(pin >= &inode_table[0] && pin < &inode_table[NR_INODE]);

    int imode = pin->i_mode & I_TYPE_MASK;
    if (imode == I_CHAR_SPECIAL) {  // check if it is for tty, same as in do_open()
        int t = fs_msg.type == READ ? DEV_READ : DEV_WRITE;
        fs_msg.type = t;

        int dev = pin->i_start_sect;
        assert(MAJOR(dev) == 4);

        fs_msg.DEVICE = MINOR(dev);
        fs_msg.BUF = buf;
        fs_msg.CNT = len;
        fs_msg.PROC_NR = src;
        assert(dd_map[MAJOR(dev)].driver_nr != INVALID_DRIVER);
        // send msg to corresponding device
        send_recv(BOTH, dd_map[MAJOR(dev)].driver_nr, &fs_msg);
        assert(fs_msg.CNT == len);

        return fs_msg.CNT;
    } else {
        assert(pin->i_mode == I_REGULAR || pin->i_mode == I_DIRECTORY);
        assert((fs_msg.type == READ) || (fs_msg.type == WRITE));

        int pos_end;  // compute the end of read/write position
        if (fs_msg.type == READ) {
            pos_end = min(pos + len, pin->i_size);
        } else {
            pos_end = min(pos + len, pin->i_nr_sects * SECTOR_SIZE); // cannot exceed sector boundary
        }
        int off = pos % SECTOR_SIZE;  
        // get start and end sector number
        int rw_sect_min = pin->i_start_sect + (pos >> SECTOR_SIZE_SHIFT); 
        int rw_sect_max = pin->i_start_sect + (pos_end >> SECTOR_SIZE_SHIFT);
        // unit size of each time read/write
        int chunk = min(rw_sect_max - rw_sect_min + 1, FSBUF_SIZE >> SECTOR_SIZE_SHIFT);  

        int bytes_rw = 0;
        int bytes_left = len;
        int i;
        for (i = rw_sect_min; i <= rw_sect_max; i += chunk) {
            // bytes each iteration read/write
            int bytes = min(bytes_left, chunk * SECTOR_SIZE - off);
            rw_sector(DEV_READ, pin->i_dev, i * SECTOR_SIZE, chunk * SECTOR_SIZE, TASK_FS, fsbuf);
            if (fs_msg.type == READ) {
                phys_copy((void*)va2la(src, buf + bytes_rw),
                          (void*)va2la(TASK_FS, fsbuf + off), 
                          bytes);
            } else {
                phys_copy((void*)va2la(TASK_FS, fsbuf + off),
                          (void*)va2la(src, buf + bytes_rw),
                          bytes);
                rw_sector(DEV_WRITE, pin->i_dev, i * SECTOR_SIZE, chunk * SECTOR_SIZE, TASK_FS, fsbuf);
            }
            off = 0;
            bytes_rw += bytes;
            pcaller->filp[fd]->fd_pos += bytes;
            bytes_left -= bytes;
        }

        // need to update file size in inode after writing
        if (pcaller->filp[fd]->fd_pos > pin->i_size) {
            pin->i_size = pcaller->filp[fd]->fd_pos;
            sync_inode(pin);
        }

        return bytes_rw;
    }
}