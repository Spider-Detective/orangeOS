#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "tty.h"
#include "console.h"
#include "proc.h"
#include "global.h"
#include "proto.h"

/* clock handler function
 * This function is used in kernel's clock int handler (kernel.asm)
 * When int happens, we will call this function in kernel stack, and shift all processes in order
 * by changing the esp pointer pointing each process table in turn
 */
PUBLIC void clock_handler(int irq) {
    ticks++;
    p_proc_ready->ticks--;

    if (k_reenter != 0) {
        return;
    }

    // use all ticks for current process, then schedule the next one
    if (p_proc_ready->ticks > 0) {
        return;
    }

    schedule();
}

/*
 * use get_ticks() to count how many secs has been passed, more accurate
 */
PUBLIC void milli_delay(int milli_sec) {
    int t = get_ticks();
    
    // count how many ticks, until it reaches milli_sec
    while ((get_ticks() - t) * 1000 / HZ < milli_sec) {}
}

PUBLIC void init_clock() {
    // initialize 8253 PIT
    out_byte(TIMER_MODE, RATE_GENERATOR);
    /* write the value of counter to be (TIMER_FREQ / HZ)
     * this makes the output freq to be 100 (HZ), and so every 10ms will have an interrupt
     */
    out_byte(TIMER0, (u8) (TIMER_FREQ / HZ));         // write lower 8-bit
    out_byte(TIMER0, (u8) ((TIMER_FREQ / HZ) >> 8));  // write higher 8-bit

    put_irq_handler(CLOCK_IRQ, clock_handler);    // register the clock int handler
    enable_irq(CLOCK_IRQ); 
}