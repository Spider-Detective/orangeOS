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
PUBLIC int search_file(char* path) {
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

