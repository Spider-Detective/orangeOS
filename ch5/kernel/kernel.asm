; NOTICE: on 64-bit VM, need to compile to specifically 32bit:
; nasm -f elf kernel.asm -o kernel.o
; ld -m elf_i386 -s kernel.o -o kernel.bin    # -s means "strip all"

SELECTOR_KERNEL_CS      equ 8

extern cstart      ; import function
extern gdt_ptr     ; import variable

[SECTION .bss]
StackSpace              resb     2 * 1024
StackTop:

[section .text]

global _start      ; output _start function from assembly

_start:   
        ; move esp from LOADER to KERNEL
        mov     esp, StackTop

        sgdt    [gdt_ptr]
        call    cstart          ; gdt_ptr is changed in this function
        lgdt    [gdt_ptr]       ; use the new gdt updated by cstart

        ; use jmp to force to use the updated gdt
        jmp     SELECTOR_KERNEL_CS:csinit   
csinit:
        push    0
        popfd      ; pop stack top into EFLAGS

        hlt