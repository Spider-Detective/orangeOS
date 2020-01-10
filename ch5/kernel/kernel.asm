; NOTICE: on 64-bit VM, need to compile to specifically 32bit:
; nasm -f elf kernel.asm -o kernel.o
; ld -m elf_i386 -s kernel.o -o kernel.bin    # -s means "strip all"

SELECTOR_KERNEL_CS      equ 8

extern cstart      
extern exception_handler

; global variables
extern gdt_ptr     
extern idt_ptr
extern disp_pos

[SECTION .bss]
StackSpace              resb     2 * 1024
StackTop:

[section .text]

global _start      ; output _start function from assembly

; all exception types are listed in Table 3.8
global	divide_error
global	single_step_exception
global	nmi
global	breakpoint_exception
global	overflow
global	bounds_check
global	inval_opcode
global	copr_not_available
global	double_fault
global	copr_seg_overrun
global	inval_tss
global	segment_not_present
global	stack_exception
global	general_protection
global	page_fault
global	copr_error

_start:   
        ; move esp from LOADER to KERNEL
        mov     esp, StackTop

		mov     dword [disp_pos], 0

        sgdt    [gdt_ptr]
        call    cstart          ; gdt_ptr is changed in this function
        lgdt    [gdt_ptr]       ; use the new gdt updated by cstart

        lidt    [idt_ptr]

        ; use jmp to force to use the updated gdt
        jmp     SELECTOR_KERNEL_CS:csinit   
csinit:
        ;ud2            ; create a exception
		jmp     0x40:0  ; create a exception with error code
        ;push    0
        ;popfd      ; pop stack top into EFLAGS

        hlt

; Exception functions, registered to idt:
; If no error code, push the placeholder error code 0xFFFFFFFF to stack
; Push the int vector to stack, then call exception_handler in protect.c
divide_error:
        push    0xFFFFFFFF      ; no error code
        push    0               ; int vector
        jmp     exception
single_step_exception:
		push	0xFFFFFFFF	
		push	1		
		jmp		exception
nmi:
		push	0xFFFFFFFF	
		push	2		
		jmp		exception
breakpoint_exception:
		push	0xFFFFFFFF	
		push	3		
		jmp		exception
overflow:
		push	0xFFFFFFFF	
		push	4		
		jmp		exception
bounds_check:
		push	0xFFFFFFFF	
		push	5		
		jmp		exception
inval_opcode:
		push	0xFFFFFFFF	
		push	6		
		jmp		exception
copr_not_available:
		push	0xFFFFFFFF
		push	7
		jmp		exception
double_fault:
		push	8		
		jmp		exception
copr_seg_overrun:
		push	0xFFFFFFFF	
		push	9		
		jmp		exception
inval_tss:
		push	10		
		jmp		exception
segment_not_present:
		push	11		
		jmp		exception
stack_exception:
		push	12		
		jmp		exception
general_protection:
		push	13		
		jmp		exception
page_fault:
		push	14		
		jmp		exception
copr_error:
		push	0xFFFFFFFF	
		push	16		
		jmp		exception

exception:
        call    exception_handler
        add     esp, 4*2           ; restore the stack, move esp to bypass pushed errorcode and int vector
                                   ; stack will be: EIP, CS, EFLAGS, see Figure 3.45
        hlt