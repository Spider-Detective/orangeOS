; NOTICE: on 64-bit VM, need to compile to specifically 32bit:
; nasm -f elf kernel.asm -o kernel.o
; ld -m elf_i386 -s kernel.o -o kernel.bin    # -s means "strip all"

SELECTOR_KERNEL_CS      equ 8

extern cstart      
extern exception_handler
extern spurious_irq

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
; interrupt types are inferred in Figure 3.39, 16 int can be registered
global hwint00
global hwint01
global hwint02
global hwint03
global hwint04
global hwint05
global hwint06
global hwint07
global hwint08
global hwint09
global hwint10
global hwint11
global hwint12
global hwint13
global hwint14
global hwint15

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
		;jmp     0x40:0  ; create a exception with error code
        ;push    0
        ;popfd      ; pop stack top into EFLAGS
		sti

        hlt

; Interruption functions for hardwares (master and slave)
; When int happens, simply call the spurious_irq in i8259.c with a int vec as parameter
%macro hwint_master    1
		push    %1
		call    spurious_irq
		add     esp, 4
		hlt
%endmacro

ALIGN   16
hwint00:                ; int handler for irq 0 (the clock)
		hwint_master    0

ALIGN   16
hwint01:                ; int handler for irq 1 (keyboard)
		hwint_master    1

ALIGN   16
hwint02:                ; int handler for irq 2 (cascade to slave)
		hwint_master    2

ALIGN   16
hwint03:                ; int handler for irq 3 (second serial)
		hwint_master    3

ALIGN   16
hwint04:                ; int handler for irq 4 (first serial)
		hwint_master    4

ALIGN   16
hwint05:                ; int handler for irq 5 (XT winchester)
		hwint_master    5

ALIGN   16
hwint06:                ; int handler for irq 6 (floppy)
		hwint_master    6

ALIGN   16
hwint07:                ; int handler for irq 7 (printer)
		hwint_master    7

%macro  hwint_slave      1
		push    %1
		call    spurious_irq
		add     esp, 4
		hlt
%endmacro

ALIGN   16
hwint08:                ; int handler for irq 8 (realtime clock)
		hwint_slave     8

ALIGN   16
hwint09:                ; int handler for irq 9 (irq 2 redirected)
		hwint_slave     9

ALIGN   16
hwint10:                ; int handler for irq 10 
		hwint_slave     10

ALIGN   16
hwint11:                ; int handler for irq 11
		hwint_slave     11

ALIGN   16
hwint12:                ; int handler for irq 12
		hwint_slave     12

ALIGN   16
hwint13:                ; int handler for irq 13 (FPU exception)
		hwint_slave     13

ALIGN   16
hwint14:                ; int handler for irq 14 (AT winchester)
		hwint_slave     14

ALIGN   16
hwint15:                ; int handler for irq 15
		hwint_slave     15


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