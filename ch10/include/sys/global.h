/* NOTICE: not define EXTERN as "extern" in global.c */
#ifdef  GLOBAL_VARIABLES_HERE
#undef  EXTERN
#define EXTERN  // define an empty value for EXTERN
#endif

EXTERN  int          ticks;
EXTERN  int          disp_pos;

EXTERN  u8                  gdt_ptr[6];       /* 0~15:Limit, 16~47:Base */
EXTERN  struct descriptor   gdt[GDT_SIZE];
EXTERN  u8                  idt_ptr[6];       /* 0~15:Limit, 16~47:Base */
EXTERN  struct gate         idt[IDT_SIZE];

EXTERN  u32          k_reenter;
EXTERN  int          current_console; // the number of current active console

EXTERN  int          key_pressed;     // wake up TASK_TTY for clock_handler

EXTERN  struct tss          tss;
EXTERN  struct proc*        p_proc_ready;

extern  char                task_stack[];
extern  struct proc         proc_table[];
extern  struct task         task_table[];
extern  struct task         user_proc_table[];
extern  irq_handler         irq_table[];
extern  TTY                 tty_table[];
extern  CONSOLE             console_table[];

/* MM */
EXTERN  MESSAGE             mm_msg;
extern  u8*                 mmbuf;
extern  const int           MMBUF_SIZE;
EXTERN  int                 memory_size;

/* FS */
EXTERN  struct file_desc    f_desc_table[NR_FILE_DESC];  // see Figure 9.12
EXTERN  struct inode        inode_table[NR_INODE];
EXTERN  struct super_block  super_block[NR_SUPER_BLOCK];
extern  u8*                 fsbuf;
extern  const int           FSBUF_SIZE;
EXTERN  MESSAGE             fs_msg;
EXTERN  struct proc*        pcaller;
EXTERN  struct inode*       root_inode;
extern  struct dev_drv_map  dd_map[];