/*
 * Implement the idt initialization and exception/interrupt handler
 */

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "proc.h"
#include "global.h"

PRIVATE void init_idt_desc(unsigned char vector, u8 desc_type,
                            int_handler handler, unsigned char privilege);
PRIVATE void init_descriptor(DESCRIPTOR* p_desc, u32 base, u32 limit, u16 attribute);

// import int handler functions from kernel.asm
// For exceptions:
void	divide_error();
void	single_step_exception();
void	nmi();
void	breakpoint_exception();
void	overflow();
void	bounds_check();
void	inval_opcode();
void	copr_not_available();
void	double_fault();
void	copr_seg_overrun();
void	inval_tss();
void	segment_not_present();
void	stack_exception();
void	general_protection();
void	page_fault();
void	copr_error();
// For hardware ints:
void    hwint00();
void    hwint01();
void    hwint02();
void    hwint03();
void    hwint04();
void    hwint05();
void    hwint06();
void    hwint07();
void    hwint08();
void    hwint09();
void    hwint10();
void    hwint11();
void    hwint12();
void    hwint13();
void    hwint14();
void    hwint15();

PUBLIC void init_prot() {
    init_8259A();

    // Initialize all exceptions to be int gate
	init_idt_desc(INT_VECTOR_DIVIDE,	  DA_386IGate,
		          divide_error,		      PRIVILEGE_KRNL);
  
	init_idt_desc(INT_VECTOR_DEBUG,		  DA_386IGate,
		          single_step_exception,  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_NMI,		  DA_386IGate,
		          nmi,			          PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_BREAKPOINT,  DA_386IGate,
		          breakpoint_exception,	  PRIVILEGE_USER);

	init_idt_desc(INT_VECTOR_OVERFLOW,	  DA_386IGate,
		          overflow,			      PRIVILEGE_USER);

	init_idt_desc(INT_VECTOR_BOUNDS,	  DA_386IGate,
		          bounds_check,		      PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_INVAL_OP,	  DA_386IGate,
		          inval_opcode,		      PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_COPROC_NOT,  DA_386IGate,
		          copr_not_available,	  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_DOUBLE_FAULT,DA_386IGate,
		          double_fault,		      PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_COPROC_SEG,  DA_386IGate,
		          copr_seg_overrun,		  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_INVAL_TSS,	  DA_386IGate,
		          inval_tss,		      PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_SEG_NOT,	  DA_386IGate,
		          segment_not_present,	  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_STACK_FAULT, DA_386IGate,
		          stack_exception,		  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_PROTECTION,  DA_386IGate,
		          general_protection,	  PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_PAGE_FAULT,  DA_386IGate,
		          page_fault,		      PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_COPROC_ERR,  DA_386IGate,
		          copr_error,		      PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ0 + 0,    DA_386IGate,
			      hwint00,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ0 + 1,    DA_386IGate,
			      hwint01,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ0 + 2,    DA_386IGate,
			      hwint02,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ0 + 3,    DA_386IGate,
			      hwint03,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ0 + 4,    DA_386IGate,
			      hwint04,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ0 + 5,    DA_386IGate,
			      hwint05,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ0 + 6,    DA_386IGate,
			      hwint06,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ0 + 7,    DA_386IGate,
			      hwint07,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 0,    DA_386IGate,
			      hwint08,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 1,    DA_386IGate,
			      hwint09,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 2,    DA_386IGate,
			      hwint10,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 3,    DA_386IGate,
			      hwint11,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 4,    DA_386IGate,
			      hwint12,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 5,    DA_386IGate,
			      hwint13,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 6,    DA_386IGate,
			      hwint14,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_IRQ8 + 7,    DA_386IGate,
			      hwint15,                PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_SYS_CALL,    DA_386IGate,
	              sys_call,               PRIVILEGE_USER);
	
	// fill TSS descriptor into GDT
	memset(&tss, 0, sizeof(tss));
	tss.ss0 = SELECTOR_KERNEL_DS;
	init_descriptor(&gdt[INDEX_TSS],
					vir2phys(seg2phys(SELECTOR_KERNEL_DS), &tss),
					sizeof(tss) - 1,
					DA_386TSS);
	tss.iobase = sizeof(tss);       // no I/O bitmap

	// fill LDT descriptor into GDT for all processes
	int i;
	PROCESS* p_proc = proc_table;
	u16 selector_ldt = INDEX_LDT_FIRST << 3;
	for (i = 0; i < NR_TASKS; i++) {
		init_descriptor(&gdt[selector_ldt >> 3],
						vir2phys(seg2phys(SELECTOR_KERNEL_DS), proc_table[i].ldts),
						LDT_SIZE * sizeof(DESCRIPTOR) - 1,
						DA_LDT);
		p_proc++;
		selector_ldt += 1 << 3;
	}
}

/* 
 * Initialize interrupt gate descriptor (defined in protect.h)
 * NOTICE: here int_handler is used, see the function pointer def in type.h
 */
PRIVATE void init_idt_desc(unsigned char vector, u8 desc_type,
                            int_handler handler, unsigned char privilege) {
    // see Code 3.36 for initialzing Gate
    GATE* p_gate = &idt[vector];
    u32   base   = (u32) handler;
    p_gate->offset_low    = base & 0xFFFF;
    p_gate->selector      = SELECTOR_KERNEL_CS;
    p_gate->dcount        = 0;
    p_gate->attr          = desc_type | (privilege << 5);
    p_gate->offset_high   = (base >> 16) & 0xFFFF;
}

/*
 * Initialize seg descriptor
 */
PRIVATE void init_descriptor(DESCRIPTOR* p_desc, u32 base, u32 limit, u16 attribute) {
	p_desc -> limit_low        = limit & 0x0FFFF;
	p_desc -> base_low         = base & 0x0FFFF;
	p_desc -> base_mid         = (base >> 16) & 0x0FF;
	p_desc -> attr1            = attribute & 0x0FF;
	p_desc -> limit_high_attr2 = ((limit >> 16) & 0x0F) | (attribute >> 8) & 0xF0;
	p_desc -> base_high        = (base >> 24) & 0x0FF;
}

/* 
 * Convert given segment to physical address using the base in gdt
 */
PUBLIC u32 seg2phys(u16 seg) {
	DESCRIPTOR* p_dest = &gdt[seg >> 3];   // each selector has length 0x8
	return (p_dest -> base_high << 24 | p_dest -> base_mid << 16 | p_dest -> base_low);
}

/*
 * Handler for exception
 * Parameters (vec_no, err_code) are pushed by functions in kernel.asm
 */
PUBLIC void exception_handler(int vec_no, int err_code, int eip, int cs, int eflags) {
    int i;
    int text_color = 0x74;   // grey back, red char

    char* err_msg[] = {
                "#DE Divide Error",
			    "#DB RESERVED",
			    "--  NMI Interrupt",
			    "#BP Breakpoint",
			    "#OF Overflow",
			    "#BR BOUND Range Exceeded",
			    "#UD Invalid Opcode (Undefined Opcode)",
			    "#NM Device Not Available (No Math Coprocessor)",
			    "#DF Double Fault",
			    "    Coprocessor Segment Overrun (reserved)",
			    "#TS Invalid TSS",
			    "#NP Segment Not Present",
			    "#SS Stack-Segment Fault",
			    "#GP General Protection",
			    "#PF Page Fault",
			    "--  (Intel reserved. Do not use.)",
			    "#MF x87 FPU Floating-Point Error (Math Fault)",
			    "#AC Alignment Check",
			    "#MC Machine Check",
			    "#XF SIMD Floating-Point Exception"
    };

    // Print spaces in first 5 lines to clear screen
    disp_pos = 0;
    for (i = 0; i < 80 * 5; i++) {
        disp_str(" ");
    }
    disp_pos = 0;

    // Print exception info, and stack status
    disp_color_str("Exception! --> ", text_color);
    disp_color_str(err_msg[vec_no], text_color);
    disp_color_str("\n\n", text_color);
    disp_color_str("EFLAGS: ", text_color);
    disp_int(eflags);
    disp_color_str("CS: ", text_color);
    disp_int(cs);
    disp_color_str("EIP: ", text_color);
    disp_int(eip);

    // Print if there is an error code
    if (err_code != 0xFFFFFFFF) {
        disp_color_str("Error code: ", text_color);
        disp_int(err_code);
    }
}