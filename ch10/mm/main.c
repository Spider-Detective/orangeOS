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

PUBLIC void do_for_test();
PRIVATE void init_mm();

PUBLIC void task_mm() {
    init_mm();
}

PRIVATE void init_mm() {
    struct boot_params bp;
    get_boot_params(&bp);

    memory_size = bp.mem_size;

    printl("{MM} memsize:%dMB\n", memory_size / (1024 * 1024));
}

PUBLIC int alloc_mem(int pid, int memsize) {
    return 0;
}

/*
 * free a memory block of a PID
 * Do nothing because we just need to remove the PID
 */
PUBLIC int free_mem(int pid) {
    return 0;
}