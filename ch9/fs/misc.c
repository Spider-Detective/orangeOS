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
 */
PUBLIC int strip_path(char* filename, const char* pathname, struct inode** ppinode) {
    return 0;
}

