
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

PUBLIC int memcmp(const void* s1, const void* s2, int n) {
    if ((s1 == 0) || (s2 == 0)) {
        return (s1 - s2);
    }

    const char* p1 = (const char*)s1;
    const char* p2 = (const char*)s2;
    for (int i = 0; i < n; i++, p1++, p2++) {
        if (*p1 != *p2) {
            return (*p1 - *p2);
        }
    }

    return 0;
}

PUBLIC int strcmp(const char* s1, const char* s2) {
    if ((s1 == 0) || (s2 == 0)) {
        return (s1 - s2);
    }

    const char* p1 = s1;
    const char* p2 = s2;
    for (; *p1 && *p2; p1++, p2++) {
        if (*p1 != *p2) {
            break;
        }
    }

    return (*p1 - *p2);
}

// concatenate s2 to the end of s1;
PUBLIC char* strcat(char* s1, const char* s2) {
    if ((s1 == 0) || (s2 == 0)) {
        return 0;
    }

    char* p1 = s1;
    for (; *p1; p1++) {}

    const char* p2 = s2;
    for (; *p2; p1++, p2++) {
        *p1 = *p2;
    }
    *p1 = 0;

    return s1;
}

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
