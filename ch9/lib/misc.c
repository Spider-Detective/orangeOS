
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

PUBLIC void spin(char* func_name) {
    printl("\nspinning in %s ...\n", func_name);
    while(1) {}
}

/*
 * Called by assert() in const.h, print out the position (line and file) of error
 */
PUBLIC void assertion_failure(char* exp, char* file, char* base_file, int line) {
    // printl macro is defined in proto.h
    printl("%c  assert(%s) failed: file: %s, base_file: %s, ln%d",
            MAG_CH_ASSERT,
            exp, file, base_file, line);

    /*
     * if assertion failed in TASK: system will halt before printl returns
     * if assertion failed in USER PROC: return from printl so we use spin to mock the halt
     */
    spin("assertion_failure()");

    __asm__ __volatile__("ud2");
}
