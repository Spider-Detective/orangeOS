#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

PUBLIC void cstart() {
    disp_str("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
             "-----\"cstart\" begins-----\n");

    // copy the GDT in LOADER into the new gdt
    memcpy(&gdt,
         (void *)(*((u32*)(&gdt_ptr[2]))),      // base of old GDT
         *((u16 *)(&gdt_ptr[0])) + 1            // limit of old GDT
           );
    
    // copy the new limit and base in gdt back to gdt_ptr
    u16* p_gdt_limit = (u16*)(&gdt_ptr[0]);
    u32* p_gdt_base = (u32*)(&gdt_ptr[2]);

    *p_gdt_limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;
    *p_gdt_base = (u32)&gdt;

    // copy the new limit and base in idt back to idt_ptr
    u16* p_idt_limit = (u16*)(&idt_ptr[0]);
    u32* p_idt_base = (u32*)(&idt_ptr[2]);

    *p_idt_limit = IDT_SIZE * sizeof(GATE) - 1;
    *p_idt_base = (u32)&idt;

    // initialize IDT and start response to interrupts here
    init_prot();

    disp_str("-----\"cstart\" ends-----\n");
}