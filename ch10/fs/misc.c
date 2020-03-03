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
#include "keyboard.h"
#include "proto.h"
#include "hd.h"

// search the given file and return inode number
// if failure, return 0
PUBLIC int search_file(char* path) {
    int i, j;

    char filename[MAX_PATH];
    memset(filename, 0, MAX_FILENAME_LEN);
    struct inode* dir_inode;
    if (strip_path(filename, path, &dir_inode) != 0) {
        return 0;
    }
    if (filename[0] == 0) {  // the path is '/'
        return dir_inode->i_num;
    }

    int dir_blk0_nr = dir_inode->i_start_sect;
    int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
    int nr_dir_entries = dir_inode->i_size / DIR_ENTRY_SIZE;

    // search the dir for the file
    int m = 0;
    struct dir_entry* pde;
    for (i = 0; i < nr_dir_blks; i++) {
        RD_SECT(dir_inode->i_dev, dir_blk0_nr + i);
        pde = (struct dir_entry*)fsbuf;
        for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++, pde++) {
            if (memcmp(filename, pde->name, MAX_FILENAME_LEN) == 0) {
                // if found the file, return its inode
                return pde->inode_nr;
            }
            if (++m > nr_dir_entries) {
                break;   // if out of the root directory range
            }
        }
        if (m > nr_dir_entries) {
            break;
        }
    }

    return 0;
}

/* get the innermost name from the fullpath
 * e.g. strip_path(filename, "/blah", ppinode) =>
 *      - filename: "blah"
 *      - *ppinode: root_inode
 *      - return 0 when success
 * *ppinode will always point to the parent folder of filename 
 * (here since only one folder, it will always be the root folder, root_inode)
 * return 0 if success, otherwise -1
 */
PUBLIC int strip_path(char* filename, const char* pathname, struct inode** ppinode) {
    const char* s = pathname;
    char* t = filename;

    if (s == 0) {
        return -1;
    }
    if (*s == '/') {
        s++;
    }

    while (*s) {
        if (*s == '/') {
            return -1;
        }
        *t++ = *s++;
        // truncate the filename if too long
        if (t - filename >= MAX_FILENAME_LEN) {
            break;
        }
    }
    *t = 0;
    *ppinode = root_inode;

    return 0;
}

