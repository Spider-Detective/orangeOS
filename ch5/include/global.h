/* NOTICE: not define EXTERN as "extern" in global.c */
#ifdef  GLOBAL_VARIABLES_HERE
#undef  EXTERN
#define EXTERN  // define an empty value for EXTERN
#endif

EXTERN  int          disp_pos;
EXTERN  u8           gdt_ptr[6];       /* 0~15:Limit, 16~47:Base */
EXTERN  DESCRIPTOR   gdt[GDT_SIZE];
EXTERN  u8           idt_ptr[6];       /* 0~15:Limit, 16~47:Base */
EXTERN  GATE         idt[IDT_SIZE];