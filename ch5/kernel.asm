; NOTICE: on 64-bit VM, need to compile to specifically 32bit:
; nasm -f elf kernel.asm -o kernel.o
; ld -m elf_i386 -s kernel.o -o kernel.bin    # -s means "strip all"

[section .text]

global _start      ; output _start function from assembly

_start:    ; assume gs already pointing to the display memory
        mov     ah, 0Fh         ; black back, white char
        mov     al, 'K'
        mov     [gs:((80 * 1 + 39) * 2)], ax
        jmp     $