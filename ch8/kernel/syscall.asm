%include "sconst.inc"

INT_VECTOR_SYS_CALL   equ 0x90
_NR_get_ticks         equ 0      ; same as the sys_call_table in global.c
_NR_write             equ 1
_NR_sendrec           equ 2

global  get_ticks
global  write
global  sendrec

bits 32
[section .text]

get_ticks:
        mov     eax, _NR_get_ticks        ; sys call request for OS
        int     INT_VECTOR_SYS_CALL
        ret

write:
        mov     eax, _NR_write
        mov     ebx, [esp + 4]
        mov     ecx, [esp + 8]
        int     INT_VECTOR_SYS_CALL
        ret

sendrec:
        mov     eax, _NR_sendrec
        mov     ebx, [esp + 4]          ; function
        mov     ecx, [esp + 8]          ; src_dest
        mov     edx, [esp + 12]         ; p_msg
        int     INT_VECTOR_SYS_CALL
        ret