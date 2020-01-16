#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"

/* clock handler function
 * This function is used in kernel's clock int handler (kernel.asm)
 * When int happens, we will call this function in kernel stack, and shift all processes in order
 * by changing the esp pointer pointing each process table in turn
 */
PUBLIC void clock_handler(int irq) {
    disp_str("#");
    p_proc_ready++;
    if (p_proc_ready >= proc_table + NR_TASKS) {
        p_proc_ready = proc_table;
    }
}