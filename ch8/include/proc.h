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

// process table
struct proc {
    struct stackframe regs;              // regs for the process saved in stack

    u16 ldt_sel;                   // gdt selector giving ldt base and limit  
    struct descriptor ldts[LDT_SIZE];     // local descriptors for code and data

    int ticks;                     // remained ticks for exe
    int priority;                  // initial ticks of the process

    u32 pid;                       // process id
    char name[16];               // name of the process

    int p_flags;
    MESSAGE* p_msg;
    int p_recvfrom;
    int p_sendto;

    int has_int_msg;
    struct proc* q_sending;
    struct proc* next_sending;

    int nr_tty;
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
#define NR_TASKS       2
#define NR_PROCS       3
#define FIRST_PROC     proc_table[0]
#define LAST_PROC      proc_table[NR_TASKS + NR_PROCS - 1]

/* stacks of tasks */
#define STACK_SIZE_TTY         0x8000
#define STACK_SIZE_SYS         0x8000
#define STACK_SIZE_TESTA       0x8000
#define STACK_SIZE_TESTB       0x8000
#define STACK_SIZE_TESTC       0x8000
#define STACK_SIZE_TOTAL       (STACK_SIZE_TTY + \
                                STACK_SIZE_SYS + \
                                STACK_SIZE_TESTA + \
                                STACK_SIZE_TESTB + \
                                STACK_SIZE_TESTC)