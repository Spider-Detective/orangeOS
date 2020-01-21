/*
 * Over all call flow for system call:
 * User process call get_ticks() (main.c)
 * INT_VECTOR_SYS_CALL in get_ticks() will trigger sys_call() in idt table (kernel.asm)
 * sys_call() calls sys_get_ticks() in sys_call_table (proc.c)
 */
#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"

PUBLIC int sys_get_ticks() {
    disp_str("+");
    return 0;
}