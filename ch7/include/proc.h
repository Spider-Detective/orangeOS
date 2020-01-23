/*
 * Constant definitions for process mechanism
 * See Figure 6.9 for the overall data structure
 */

// regs on stack
typedef struct s_stackframe {
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
} STACK_FRAME;

// process table
typedef struct s_proc {
    STACK_FRAME regs;              // regs for the process saved in stack

    u16 ldt_sel;                   // gdt selector giving ldt base and limit  
    DESCRIPTOR ldts[LDT_SIZE];     // local descriptors for code and data

    int ticks;                     // remained ticks for exe
    int priority;                  // initial ticks of the process

    u32 pid;                       // process id
    char p_name[16];               // name of the process
} PROCESS;

// data structure to store the initilization info of a task/process, to be put into process table
typedef struct s_task {
    task_f  initial_eip;
    int     stacksize;
    char    name[32];
} TASK;

/* Number of tasks */
#define NR_TASKS       3

/* stacks of tasks */
#define STACK_SIZE_TESTA       0x8000
#define STACK_SIZE_TESTB       0x8000
#define STACK_SIZE_TESTC       0x8000
#define STACK_SIZE_TOTAL       (STACK_SIZE_TESTA + STACK_SIZE_TESTB + STACK_SIZE_TESTC)