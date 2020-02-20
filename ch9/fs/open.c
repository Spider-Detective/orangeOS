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

PRIVATE struct inode* create_file(char* path, int flags);
PRIVATE int alloc_imap_bit(int dev);
PRIVATE int alloc_smap_bit(int dev, int nr_sects_to_alloc);
PRIVATE struct inode* new_inode(int dev, int inode_nr, int start_sect);
PRIVATE void new_dir_entry(struct inode* dir_inode, int inode_nr, char* filename);

// open the file and return a fd int
PUBLIC int do_open() {
    return 0;
}

/*
 * Create a file and return its inode pointer
 * path: the full path of the created file
 * flags: the attributes of the created file
 * return: pointer to the inode if success, otherwise 0
 */
PUBLIC struct inode* create_file(char* path, int flags) {
    char filename[MAX_PATH];
    struct inode* dir_inode;

    if (strip_path(filename, path, &dir_inode) != 0) {
        return 0;
    }

    int inode_nr = alloc_imap_bit(dir_inode->i_dev);
    int free_sect_nr = alloc_smap_bit(dir_inode->i_dev, NR_DEFAULT_FILE_SECTS);
    struct inode* newino = new_inode(dir_inode->i_dev, inode_nr, free_sect_nr);
    new_dir_entry(dir_inode, newino->i_num, filename);

    return newino;
}

PUBLIC int do_close() {
    int fd = fs_msg.FD;
    put_inode(pcaller->filp[fd]->fd_inode);
    pcaller->filp[fd]->fd_inode = 0;
    pcaller->filp[fd] = 0;

    return 0;
}

PUBLIC int do_lseek() {

}

// allocate the bit for the file to create, in inode map
// return the inode number
PRIVATE int alloc_imap_bit(int dev) {

}

// allocate the bit for the file to create, in sector map
// return the first allocated sector number
PRIVATE int alloc_smap_bit(int dev, int nr_sects_to_alloc) {

}

// create a new inode and write it to disk
// return the newly-created inode
PRIVATE struct inode* new_inode(int dev, int inode_nr, int start_sect) {

}

// write a new entry into the directory
PRIVATE void new_dir_entry(struct inode* dir_inode, int inode_nr, char* filename) {
    
}