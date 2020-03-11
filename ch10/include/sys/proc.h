/*
 * Constant definitions for process mechanism
 * See Figure 6.9 for the overall data structure
 */

// regs on stack
struct stackframe {
    u32    gs;
    u32    fs;
    u32    es;
    u32    ds;
    u32    edi;
    u32    esi;
    u32    ebp;
    u32    kernel_esp;
    u32    ebx;
    u32    edx;
    u32    ecx;
    u32    eax;
    u32    retaddr;      // return addr
    u32    eip;          // the last 5 is pushed by CPU then interrupt
    u32    cs;
    u32    eflags;
    u32    esp;
    u32    ss;
};

// process table, store the info for each process
struct proc {
    struct stackframe regs;              // regs for the process saved in stack

    u16 ldt_sel;                   // gdt selector giving ldt base and limit  
    struct descriptor ldts[LDT_SIZE];     // local descriptors for code and data

    int ticks;                     // remained ticks for exe
    int priority;                  // initial ticks of the process

    // u32 pid;                       // process id
    char name[16];               // name of the process

    int p_flags;                  // a process is runnable iff p_flags == 0
    MESSAGE* p_msg;
    int p_recvfrom;
    int p_sendto;

    int has_int_msg;
    struct proc* q_sending;        // queue of procs wanting to send msg to this proc
    struct proc* next_sending;     // next proc in the sending queue, linked list

    int p_parent;                  // parent process's pid
    int exit_status;               // returned to parent

    struct file_desc* filp[NR_FILES];   // stores the array of file descriptor
};

// data structure to store the initilization info of a task/process, to be put into process table
struct task {
    task_f  initial_eip;
    int     stacksize;
    char    name[32];
};

#define proc2pid(x) (x - proc_table)

/* Number of tasks and processes, 
 * tasks: tty etc, run on ring1
 * processes: for user, run on ring3
 */
#define NR_TASKS         5
#define NR_PROCS         32
#define NR_NATIVE_PROCS  4
#define FIRST_PROC       proc_table[0]
#define LAST_PROC        proc_table[NR_TASKS + NR_PROCS - 1]

// all forked procs will use memory above PROCS_BASE
#define PROCS_BASE              0xA00000    // 10 MB
#define PROC_IMAGE_SIZE_DEFAULT 0x100000    // 1MB
#define PROC_ORIGIN_STACK       0x400       // 1KB

/* stacks of tasks */
#define STACK_SIZE_DEFAULT     0x4000      // 16 KB
#define STACK_SIZE_TTY         STACK_SIZE_DEFAULT
#define STACK_SIZE_SYS         STACK_SIZE_DEFAULT
#define STACK_SIZE_HD          STACK_SIZE_DEFAULT
#define STACK_SIZE_FS          STACK_SIZE_DEFAULT
#define STACK_SIZE_MM          STACK_SIZE_DEFAULT
#define STACK_SIZE_INIT        STACK_SIZE_DEFAULT

#define STACK_SIZE_TESTA       STACK_SIZE_DEFAULT
#define STACK_SIZE_TESTB       STACK_SIZE_DEFAULT
#define STACK_SIZE_TESTC       STACK_SIZE_DEFAULT

#define STACK_SIZE_TOTAL       (STACK_SIZE_TTY + \
                                STACK_SIZE_SYS + \
                                STACK_SIZE_HD + \
                                STACK_SIZE_FS + \
                                STACK_SIZE_MM + \
                                STACK_SIZE_INIT + \
                                STACK_SIZE_TESTA + \
                                STACK_SIZE_TESTB + \
                                STACK_SIZE_TESTC)