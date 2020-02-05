#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"

/*
 * use fmt as the starting point
 * e.g. printf(fmt, i, j):  
 *      push j
 *      push i
 *      push fmt
 *      call printf
 *      add  esp, 3 * 4
 */
int printf(const char *fmt, ...) {
    int i;
    char buf[256];

    va_list arg = (va_list)((char *)(&fmt) + 4);    // 4 is the size of fmt on stack
    i = vsprintf(buf, fmt, arg);
    buf[i] = 0;
    printx(buf);     // system call, calls printx in syscall.asm

    return i;
}