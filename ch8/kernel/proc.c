/*
 * Over all call flow for system call:
 * User process call get_ticks() (main.c)
 * INT_VECTOR_SYS_CALL in get_ticks() will trigger sys_call() in idt table (kernel.asm)
 * sys_call() calls sys_get_ticks() in sys_call_table (proc.c)
 */
#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

PUBLIC int sys_get_ticks() {
    return ticks;
}

/*
 * Function for scheduling the processes
 */
PUBLIC void schedule() {
    PROCESS* p;
    int      greatest_ticks = 0;

    while (!greatest_ticks) {
        // switch to the process with largest ticks
        for (p = proc_table; p < (proc_table + NR_TASKS + NR_PROCS); p++) {
            if (greatest_ticks < p->ticks) {
                greatest_ticks = p->ticks;
                p_proc_ready = p;
            }
        }
        // if all processes have 0 ticks, 
        // assign the initial ticks to all processes and switch to the largest one
        if (!greatest_ticks) {
            for (p = proc_table; p < (proc_table + NR_TASKS + NR_PROCS); p++) {
                p->ticks = p->priority;
            }
        }
    }
}