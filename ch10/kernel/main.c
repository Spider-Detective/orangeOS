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
    disp_str("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
             "-----\"kernel_main\" begins-----\n");

    // prepare the process table for all processes
    u8 privilege;
    u8 rpl;
    int eflags, prio;       // priority of the process
    int i, j;  

    struct task* t;
    struct proc* p = proc_table;
    char* stk = task_stack + STACK_SIZE_TOTAL;

    for (i = 0; i < NR_TASKS + NR_PROCS; i++, p++, t++) {
        if (i >= NR_TASKS + NR_NATIVE_PROCS) {
            p->p_flags = FREE_SLOT;
            continue;
        }

        // initialize task and proc differently
        if (i < NR_TASKS) {
            t          = task_table + i;
            privilege  = PRIVILEGE_TASK;
            rpl        = RPL_TASK;
            eflags     = 0x1202;     /* IF=1, IOPL=1, bit 2 is always 1 */ 
            prio       = 15;
        } else {
            t          = user_proc_table + (i - NR_TASKS);
            privilege  = PRIVILEGE_USER;
            rpl        = RPL_USER;
            eflags     = 0x202;      /* IF=1, bit 2 is always 1 */
            prio       = 5;
        }

        strcpy(p->name, t->name);       // process name
        p->p_parent = NO_TASK;

        if (strcmp(t->name, "INIT") != 0) {
            p->ldts[INDEX_LDT_C] = gdt[SELECTOR_KERNEL_CS >> 3];
            p->ldts[INDEX_LDT_RW] = gdt[SELECTOR_KERNEL_DS >> 3];
            p->ldts[INDEX_LDT_C].attr1  = DA_C   | privilege << 5; // downgrade DPL to exe on ring1
            p->ldts[INDEX_LDT_RW].attr1 = DA_DRW | privilege << 5;
        } else {    // INIT process
            u32 k_base;
            u32 k_limit;
            int ret = get_kernel_map(&k_base, &k_limit);
            assert(ret == 0);
            init_desc(&p->ldts[INDEX_LDT_C],
                      0,
                      (k_base + k_limit) >> LIMIT_4K_SHIFT,
                      DA_32 | DA_LIMIT_4K | DA_C | privilege << 5);
            init_desc(&p->ldts[INDEX_LDT_RW],
                      0,
                      (k_base + k_limit) >> LIMIT_4K_SHIFT,
                      DA_32 | DA_LIMIT_4K | DA_DRW | privilege << 5);
        }

        p->regs.cs = INDEX_LDT_C << 3 | SA_TIL | rpl;
        p->regs.ds = p->regs.es = p->regs.fs = p->regs.ss = INDEX_LDT_RW << 3 | SA_TIL | rpl;
        p->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;
        p->regs.eip = (u32)t->initial_eip;
        p->regs.esp = (u32)stk;
        p->regs.eflags = eflags;

        p->ticks = p->priority = prio;

        p->p_flags = 0;
        p->p_msg   = 0;
        p->p_recvfrom = NO_TASK;
        p->p_sendto   = NO_TASK;
        p->has_int_msg  = 0;
        p->q_sending    = 0;
        p->next_sending = 0;

        for (j = 0; j < NR_FILES; j++) {
            p->filp[j] = 0;
        }
        
        stk -= t->stacksize;
    }

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

// The first process
void Init() {
    int fd_stdin = open("/dev_tty0", O_RDWR);
    assert(fd_stdin == 0);
    int fd_stdout = open("/dev_tty0", O_RDWR);
    assert(fd_stdout == 1);

    printf("Init() is running...\n");

    int pid = fork();
    if (pid != 0) {   // parent process
        printf("parent is running, child pid:%d\n", pid);
        spin("parent");
    } else {          // child process
        printf("child is running, pid:%d\n", getpid());
        spin("child");
    }
}

/* Simple function of process A
 * This process is run on ring1
 * When kernel starts, it uses restart() to do ring0 -> ring1 thus run TestA()
 * When interrupt happens, suspend TestA and exe int handler, ring1 -> ring0
 * esp0 in tss is prepared, to ensure ring1 -> ring0 is ok (the preparation is done
 * in restart() and the handler: mov dword [tss + TSS3_S_SP0], eax)
 */
void TestA() {
    for (;;);
    // int fd;
    // int i, n;
    // char filename[MAX_FILENAME_LEN + 1] = "blah";
    // const char bufw[] = "abcde";  // buffer to write
    // const int rd_bytes = 3;
    // char bufr[rd_bytes];          // buffer to read
    // assert(rd_bytes <= strlen(bufw));

    // // create file
    // fd = open(filename, O_CREAT | O_RDWR);  // call function in lib/open.c
    // assert(fd != -1);
    // printl("File created: %s (fd %d)\n", filename, fd);

    // // write file
    // n = write(fd, bufw, strlen(bufw));
    // assert(n == strlen(bufw));
    // close(fd);

    // // open file
    // fd = open(filename, O_RDWR);
    // assert(fd != -1);
    // printl("File opened. fd: %d\n", fd);

    // // read file
    // n = read(fd, bufr, rd_bytes);
    // assert(n == rd_bytes);
    // bufr[n] = 0;
    // printl("%d bytes read: %s\n", n, bufr);

    // // close file
    // close(fd);

    // // try create and remove files
    // char* filenames[] = {"/foo", "/bar", "/baz"};
    // for (i = 0; i < sizeof(filenames) / sizeof(filenames[0]); i++) {
    //     fd = open(filenames[i], O_CREAT | O_RDWR);
    //     assert(fd != -1);
    //     printl("File created: %s (fd %d)\n", filenames[i], fd);
    //     close(fd);
    // }

    // char* rfilenames[] = {"/bar", "/foo", "/baz", "/dev_tty0"};
    // for (i = 0; i < sizeof(rfilenames) / sizeof(rfilenames[0]); i++) {
    //     if (unlink(rfilenames[i]) == 0) {
    //         printl("File removed: %s\n", rfilenames[i]);
    //     } else {
    //         printl("Failed to remove file: %s\n", rfilenames[i]);
    //     }
    // }

    // spin("TestA");
}

// Another process B
void TestB() {
    for (;;);
    // char tty_name[] = "/dev_tty1";
    // int fd_stdin = open(tty_name, O_RDWR);
    // assert(fd_stdin == 0);
    // int fd_stdout = open(tty_name, O_RDWR);
    // assert(fd_stdout == 1);

    // char rdbuf[128];   // read buffer

    // while(1) {
    //     printf("$ ");
    //     int r = read(fd_stdin, rdbuf, 70);
    //     rdbuf[r] = 0;

    //     if (strcmp(rdbuf, "hello") == 0) {
    //         printf("hello world!\n");
    //     } else {
    //         if (rdbuf[0]) {
    //             printf("{%s}\n", rdbuf); 
    //         }
    //     }
    // }

    // assert(0);
}

void TestC() {
    for (;;);
    // spin("TestC");
    // assert(0);
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