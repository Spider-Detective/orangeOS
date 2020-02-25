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
#include "proto.h"

/*
 * This function is to prepare process table and start a process
 */
PUBLIC int kernel_main() {
    disp_str("-----\"kernel_main\" begins-----\n");

    // prepare the process table for all processes
    struct task*      p_task;
    struct proc*      p_proc        = proc_table;
    char*             p_task_stack  = task_stack + STACK_SIZE_TOTAL;
    u16               selector_ldt  = SELECTOR_LDT_FIRST; 
    u8                privilege;
    u8                rpl;
    int               eflags;
    int               prio;       // priority of the process

    int i;  
    for (i = 0; i < NR_TASKS + NR_PROCS; i++) {
        // initialize task and proc differently
        if (i < NR_TASKS) {
            p_task     = task_table + i;
            privilege  = PRIVILEGE_TASK;
            rpl        = RPL_TASK;
            eflags     = 0x1202;     /* IF=1, IOPL=1, bit 2 is always 1 */ 
            prio       = 15;
        } else {
            p_task     = user_proc_table + (i - NR_TASKS);
            privilege  = PRIVILEGE_USER;
            rpl        = RPL_USER;
            eflags     = 0x202;      /* IF=1, bit 2 is always 1 */
            prio       = 5;
        }

        strcpy(p_proc->name, p_task->name);       // process name
        p_proc->pid = i;                            // process id

        p_proc->ldt_sel = selector_ldt;

        memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(struct descriptor));
        p_proc->ldts[0].attr1 = DA_C | privilege << 5;  // downgrade DPL to exe on ring1
        memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(struct descriptor));
        p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;  // downgrade DPL to exe on ring1

        p_proc->regs.cs = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl; // first LDT descriptor
        p_proc->regs.ds = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl; // second LDT descriptor
        p_proc->regs.es = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
        p_proc->regs.fs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
        p_proc->regs.ss = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
        p_proc->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;

        p_proc->regs.eip = (u32)p_task->initial_eip;     // entry point when iretd, exe process function
        p_proc->regs.esp = (u32)p_task_stack;
        p_proc->regs.eflags = eflags;

        p_proc->nr_tty = 0;    

        p_proc->p_flags = 0;
        p_proc->p_msg   = 0;
        p_proc->p_recvfrom = NO_TASK;
        p_proc->p_sendto   = NO_TASK;
        p_proc->has_int_msg  = 0;
        p_proc->q_sending    = 0;
        p_proc->next_sending = 0;
        p_proc->ticks = p_proc->priority = prio;

        p_task_stack -= p_task->stacksize;
        p_proc++;
        p_task++;
        selector_ldt += 1 << 3;
    }
    
    proc_table[NR_TASKS + 0].nr_tty = 0;   // console output will be on console #0
    proc_table[NR_TASKS + 1].nr_tty = 1;
    proc_table[NR_TASKS + 2].nr_tty = 1;

    k_reenter = 0;
    ticks = 0;

    // start the process
    p_proc_ready = proc_table;

    init_clock();                       // enable the clock int
    init_keyboard();

    restart();

    while (1) {}
}

PUBLIC int get_ticks() {
    MESSAGE msg;
    reset_msg(&msg);
    msg.type = GET_TICKS;
    send_recv(BOTH, TASK_SYS, &msg);
    return msg.RETVAL;
}

/* Simple function of process A
 * This process is run on ring1
 * When kernel starts, it uses restart() to do ring0 -> ring1 thus run TestA()
 * When interrupt happens, suspend TestA and exe int handler, ring1 -> ring0
 * esp0 in tss is prepared, to ensure ring1 -> ring0 is ok (the preparation is done
 * in restart() and the handler: mov dword [tss + TSS3_S_SP0], eax)
 */
void TestA() {
    int fd;
    int n;
    const char filename[] = "blah";
    const char bufw[] = "abcde";  // buffer to write
    const int rd_bytes = 3;
    char bufr[rd_bytes];          // buffer to read
    assert(rd_bytes <= strlen(bufw));

    // create file
    fd = open(filename, O_CREAT | O_RDWR);  // call function in lib/open.c
    assert(fd != -1);
    printf("File created. fd: %d\n", fd);

    // write file
    n = write(fd, bufw, strlen(bufw));
    assert(n == strlen(bufw));
    close(fd);

    // open file
    fd = open(filename, O_RDWR);
    assert(fd != -1);
    printf("File opened. fd: %d\n", fd);

    // read file
    n = read(fd, bufr, rd_bytes);
    assert(n == rd_bytes);
    bufr[n] = 0;
    printf("%d bytes read: %s\n", n, bufr);

    // close file
    close(fd);

    spin("TestA");
}

// Another process B
void TestB() {
    while(1) {
        printf("B");
        milli_delay(200);
    }
}

void TestC() {
    // assert(0);
    while(1) {
        printf("C");
        milli_delay(200);
    }
}

PUBLIC void panic(const char *fmt, ...) {
    int i;
    char buf[256];

    // 4 is the size of fmt on stack, jump over it
    va_list arg = (va_list)((char *)&fmt + 4);

    i = vsprintf(buf, fmt, arg);

    printl("%c !!panic!! %s", MAG_CH_PANIC, buf);

    __asm__ __volatile__("ud2");     // Raise invalid opcode exception
}