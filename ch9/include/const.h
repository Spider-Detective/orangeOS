
#ifndef _ORANGES_CONST_H_
#define _ORANGES_CONST_H_

#define ASSERT
#ifdef  ASSERT
void assertion_failure(char* exp, char* file, char* base_file, int line);
#define assert(exp)  if (exp) ; \
                else assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)
#else
#define assert(exp)
#endif
/* EXTERN is defined as "extern" except in global.c, see global.h */
#define EXTERN extern

// function types
#define PUBLIC
#define PRIVATE static

#define STR_DEFAULT_LEN 1024

#define max(a, b)    (a > b ? a : b)
#define min(a, b)    (a < b ? a : b)

#define TRUE    1
#define FALSE   0

/* COLOR DEFINITIONS 
 * e.g. MAKECOLOR(BLACK | RED) | BRIGHT
 */
#define BLACK   0x0
#define WHITE   0x7
#define RED     0x4
#define GREEN   0x2
#define BLUE    0x1
#define FLASH   0x80
#define BRIGHT  0x08
#define MAKE_COLOR(x, y) (x | y)    /* (background, foreground) */

#define GDT_SIZE        128
#define IDT_SIZE        256

// privilege
#define PRIVILEGE_KRNL  0
#define PRIVILEGE_TASK  1
#define PRIVILEGE_USER  3

// RPL 
#define RPL_KRNL        SA_RPL0
#define RPL_TASK        SA_RPL1
#define RPL_USER        SA_RPL3

/* Process */
#define SENDING        0x02      /* set when proc trying to send */
#define RECEIVING      0x04      /* set when proc trying to receive */

/* TTY */
#define NR_CONSOLES    3     /* consoles, 32kb video mem is enough for 3 80*25 consoles */

/* 8259A interrupt controller ports */
#define INT_M_CTL      0x20  /* I/O port for interrupt controller       <Master> */
#define INT_M_CTLMASK  0x21  /* Setting bits in this port disables ints <Master> */
#define INT_S_CTL      0xA0  /* I/O port for 2nd interrupt controller   <Slave>  */
#define INT_S_CTLMASK  0xA1  /* Setting bits in this port disables ints <Slave>  */

/* 8253/8254 PIT (Programmable Interval Timer), see Section 6.5.2.1 */
#define TIMER0         0x40       /* I/O port for timer channel 0 */
#define TIMER_MODE     0x43       /* I/O port for timer mode control */
#define RATE_GENERATOR 0x34       /* 00-11-010-0: Counter 0 - LSB then MSB - mode 2 - bonary, see Figure 6.24 */
#define TIMER_FREQ     1193182L   /* given clock frequency for timer in PC */
#define HZ             100        /* target clock freq we want to set */

/* AT-type keyboard, 8042 ports, see Table 7.1 */
#define KB_DATA        0x60   /* I/O port for kayboard data (read/write) */
#define KB_CMD         0x64   /* I/O port for keyboard command (read status register/write input buffer) */
#define LED_CODE       0xED   /* Command to write to LED lights of keyboard */
#define KB_ACK         0xFA   /* ACK from Keyboard */

/* VGA, see Table 7.4 and 7.5 */
#define CRTC_ADDR_REG  0x3D4   /* Addr Register */
#define CRTC_DATA_REG  0x3D5   /* Data Register */
#define START_ADDR_H   0xC     /* Video mem start addr */
#define START_ADDR_L   0xD     /* Video mem start addr */
#define CURSOR_H       0xE     /* Cursor posision */
#define CURSOR_L       0xF     /* Cursor position */
#define V_MEM_BASE     0xB8000 /* Base of color video mem */
#define V_MEM_SIZE     0x8000  /* 32K */

/* Hardware interrupts */
#define NR_IRQ          16
#define CLOCK_IRQ       0
#define KEYBOARD_IRQ    1
#define CASCADE_IRQ     2      /* cascade to second AT controller */
#define ETHER_IRQ       3      
#define SECONDARY_IRQ   3      /* RS232 int vec for port 2 */ 
#define RS232_IRQ       4      /* RS232 int vec for port 1 */
#define XT_WINI_IRQ     5      /* XT winchester */
#define FLOPPY_IRQ      6
#define PRINTER_IRQ     7
#define AT_WINI_IRQ     14     /* AT winchester */

/* tasks */
/* TASK_* has the same definition as in global.c */
#define INVALID_DRIVER	-20
#define INTERRUPT	    -10
#define TASK_TTY	    0
#define TASK_SYS	    1
#define TASK_HD     	2
/* #define TASK_FS	3 */
/* #define TASK_MM	4 */
#define ANY		        (NR_TASKS + NR_PROCS + 10)
#define NO_TASK		    (NR_TASKS + NR_PROCS + 20)

#define NR_SYS_CALL     3

/* ipc consts */
#define SEND            1
#define RECEIVE         2
#define BOTH            3   /* (SEND | RECEIVE) */

/* 'magic chars' used by 'printx' */
#define MAG_CH_PANIC    '\002'
#define MAG_CH_ASSERT   '\003'

enum msgtype {
    HARD_INT = 1,
    GET_TICKS,

    /* msg type for drivers */
    DEV_OPEN = 1001,
    DEV_CLOSE,
    DEV_READ,
    DEV_WRITE,
    DEV_IOCTL
};

#define CNT             u.m3.m3i2
#define REQUEST         u.m3.m3i2
#define PROC_NR         u.m3.m3i3
#define DEVICE          u.m3.m3i4
#define POSITION        u.m3.m3l1
#define BUF             u.m3.m3p2
#define RETVAL          u.m3.m3i1




#define	DIOCTL_GET_GEO	1

/* Hard Drive */
#define SECTOR_SIZE		512
#define SECTOR_BITS		(SECTOR_SIZE * 8)
#define SECTOR_SIZE_SHIFT	9

/* major device numbers (corresponding to kernel/global.c::dd_map[]) */
#define	NO_DEV			0
#define	DEV_FLOPPY		1
#define	DEV_CDROM		2
#define	DEV_HD			3
#define	DEV_CHAR_TTY	4
#define	DEV_SCSI		5
/* make device number from major and minor numbers */
#define	MAJOR_SHIFT		8
#define	MAKE_DEV(a,b)	((a << MAJOR_SHIFT) | b)
/* separate major and minor numbers from device number */
#define	MAJOR(x)		((x >> MAJOR_SHIFT) & 0xFF)
#define	MINOR(x)		(x & 0xFF)

/* device numbers of hard disk */
#define	MINOR_hd1a		0x10
#define	MINOR_hd2a		0x20
#define	MINOR_hd2b		0x21
#define	MINOR_hd3a		0x30
#define	MINOR_hd4a		0x40

#define	ROOT_DEV		MAKE_DEV(DEV_HD, MINOR_BOOT)	/* 3, 0x21 */

#define	INVALID_INODE		0
#define	ROOT_INODE		    1

#define	MAX_DRIVES		    2
#define	NR_PART_PER_DRIVE	4
#define	NR_SUB_PER_PART		16
#define	NR_SUB_PER_DRIVE	(NR_SUB_PER_PART * NR_PART_PER_DRIVE)
#define	NR_PRIM_PER_DRIVE	(NR_PART_PER_DRIVE + 1)

/**
 * @def MAX_PRIM_DEV
 * Defines the max minor number of the primary partitions.
 * If there are 2 disks, prim_dev ranges in hd[0-9], this macro will
 * equals 9.
 */
#define	MAX_PRIM		(MAX_DRIVES * NR_PRIM_PER_DRIVE - 1)

#define	MAX_SUBPARTITIONS	(NR_SUB_PER_DRIVE * MAX_DRIVES)

#define	P_PRIMARY	0
#define	P_EXTENDED	1

#define ORANGES_PART	0x99	/* Orange'S partition */
#define NO_PART		0x00	/* unused entry */
#define EXT_PART	0x05	/* extended partition */

#define	NR_FILES	64
#define	NR_FILE_DESC	64	/* FIXME */
#define	NR_INODE	64	/* FIXME */
#define	NR_SUPER_BLOCK	8


/* INODE::i_mode (octal, lower 32 bits reserved) */
#define I_TYPE_MASK     0170000
#define I_REGULAR       0100000
#define I_BLOCK_SPECIAL 0060000
#define I_DIRECTORY     0040000
#define I_CHAR_SPECIAL  0020000
#define I_NAMED_PIPE	0010000

#define	is_special(m)	((((m) & I_TYPE_MASK) == I_BLOCK_SPECIAL) ||	\
			 (((m) & I_TYPE_MASK) == I_CHAR_SPECIAL))

#define	NR_DEFAULT_FILE_SECTS	2048 /* 2048 * 512 = 1MB */

#endif /* _ORANGES_CONST_H_ */