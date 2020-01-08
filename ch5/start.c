#include "type.h"
#include "const.h"
#include "protect.h"

PUBLIC  void* memcpy(void* pDst, void* pSrc, int iSize);
PUBLIC  void disp_str(char* pszInfo);

PUBLIC  u8          gdt_ptr[6];       /* 0~15:Limit, 16~47:Base */
PUBLIC DESCRIPTOR   gdt[GDT_SIZE];

PUBLIC void cstart() {
    disp_str("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
             "-----\"cstart\" begins-----");

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
}