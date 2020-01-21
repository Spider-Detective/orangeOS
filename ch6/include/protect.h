// defines the data structure used in protect mode

#ifndef _ORANGES_PROTECT_H_
#define _ORANGES_PROTECT_H_

// descriptor for data/system seg, 8 bytes
// refer to pm.inc
typedef struct s_descriptor {
    u16    limit_low;
    u16    base_low;
    u8     base_mid;
    u8     attr1;
    u8     limit_high_attr2;
    u8     base_high
}DESCRIPTOR;

// gate descriptor, see Code 3.40
typedef struct s_gate {
    u16    offset_low;
    u16    selector;
    u8     dcount;
    u8     attr;
    u16    offset_high;
}GATE;

/*
 * es0: we need to set esp0 to the stack in current process table before ring1 -> ring0
 */
typedef struct s_tss {
    u32    backlink;
    u32    esp0;         // stack pointer to use when interrupt
    u32    ss0;
    u32    esp1;
    u32    ss1;
    u32    esp2;
    u32    ss2;
    u32    cr3;
    u32    eip;
    u32    flags;
    u32    eax;
    u32    ecx;
    u32    edx;
    u32    ebx;
    u32    esp;
    u32    ebp;
    u32    esi;
    u32    edi;
    u32    es;
    u32    cs;
    u32    ds;
    u32    fs;
    u32    gs;
    u32    ldt;
    u16    trap;
    u16    iobase
} TSS;

/* GDT */
/* Descriptor indexes and selectors, set in loader.asm */
#define	INDEX_DUMMY		    0
#define	INDEX_FLAT_C		1	
#define	INDEX_FLAT_RW		2	
#define	INDEX_VIDEO		    3	
#define INDEX_TSS           4
#define INDEX_LDT_FIRST     5

#define	SELECTOR_DUMMY		   0		
#define	SELECTOR_FLAT_C		0x08		
#define	SELECTOR_FLAT_RW	0x10		
#define	SELECTOR_VIDEO		(0x18+3)	
#define SELECTOR_TSS        0x20
#define SELECTOR_LDT_FIRST  0x28

#define	SELECTOR_KERNEL_CS	SELECTOR_FLAT_C
#define	SELECTOR_KERNEL_DS	SELECTOR_FLAT_RW
#define SELECTOR_KERNEL_GS  SELECTOR_VIDEO

#define LDT_SIZE            2

/* Selector Attribute (SA) */
#define SA_RPL_MASK     0xFFFC
#define SA_RPL0         0
#define SA_RPL1         1
#define SA_RPL2         2
#define SA_RPL3         3

#define SA_TI_MASK      0xFFFB
#define SA_TIG          0
#define SA_TIL          4

/* Descriptor Attr */
#define	DA_32			0x4000	/* 32-bit seg				*/
#define	DA_LIMIT_4K		0x8000	/* seg limit granulairty is 4K		*/
#define	DA_DPL0			0x00	/* DPL = 0				*/
#define	DA_DPL1			0x20	/* DPL = 1				*/
#define	DA_DPL2			0x40	/* DPL = 2				*/
#define	DA_DPL3			0x60	/* DPL = 3				*/
/* Attr for data seg */
#define	DA_DR			0x90	/* Readonly		*/
#define	DA_DRW			0x92	/* Read and write only		*/
#define	DA_DRWA			0x93	/* Read and write only, visited	*/
#define	DA_C			0x98	/* Execute-only		*/
#define	DA_CR			0x9A	/* Execute and read only		*/
#define	DA_CCO			0x9C	/* Execute-only, consistent code seg		*/
#define	DA_CCOR			0x9E	/* Execute and read only, consistent code seg	*/
/* Attr for system seg */
#define	DA_LDT			0x82	
#define	DA_TaskGate		0x85	
#define	DA_386TSS		0x89	
#define	DA_386CGate		0x8C	
#define	DA_386IGate		0x8E	
#define	DA_386TGate		0x8F	

/* Interrupt vectors */
#define	INT_VECTOR_DIVIDE		    0x0
#define	INT_VECTOR_DEBUG		    0x1
#define	INT_VECTOR_NMI			    0x2
#define	INT_VECTOR_BREAKPOINT	    0x3
#define	INT_VECTOR_OVERFLOW		    0x4
#define	INT_VECTOR_BOUNDS		    0x5
#define	INT_VECTOR_INVAL_OP		    0x6
#define	INT_VECTOR_COPROC_NOT		0x7
#define	INT_VECTOR_DOUBLE_FAULT		0x8
#define	INT_VECTOR_COPROC_SEG		0x9
#define	INT_VECTOR_INVAL_TSS		0xA
#define	INT_VECTOR_SEG_NOT		    0xB
#define	INT_VECTOR_STACK_FAULT		0xC
#define	INT_VECTOR_PROTECTION		0xD
#define	INT_VECTOR_PAGE_FAULT		0xE
#define	INT_VECTOR_COPROC_ERR		0x10

// Interrupt vectors
#define INT_VECTOR_IRQ0        0x20
#define INT_VECTOR_IRQ8        0x28

/* system calls */
#define INT_VECTOR_SYS_CALL    0x90

// macros
#define vir2phys(seg_base, vir) (u32)(((u32)seg_base) + ((u32)vir))

#endif /* _ORANGES_PROTECT_H_ */