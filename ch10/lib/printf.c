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

// used by user calls, use write()
// NOTICE: assume the console file is opened, and fd is always 1
PUBLIC int printf(const char* fmt, ...) {
    int i;
    char buf[STR_DEFAULT_LEN];

    va_list arg = (va_list)((char *)(&fmt) + 4);    // 4 is the size of fmt on stack
    i = vsprintf(buf, fmt, arg);
    int c = write(1, buf, i);

    assert(c == i);

    return i;
}

/*
 * low level print, used in kernel and syscalls
 * use fmt as the starting point
 * e.g. printf(fmt, i, j):  
 *      push j
 *      push i
 *      push fmt
 *      call printf
 *      add  esp, 3 * 4
 */
PUBLIC int printl(const char *fmt, ...) {
    int i;
    char buf[STR_DEFAULT_LEN];

    va_list arg = (va_list)((char *)(&fmt) + 4);    // 4 is the size of fmt on stack
    i = vsprintf(buf, fmt, arg);  // de-format the actual string to print
    printx(buf);     // system call, calls printx in syscall.asm

    return i;
}