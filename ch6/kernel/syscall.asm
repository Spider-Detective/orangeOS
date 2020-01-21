%include "sconst.inc"

_NR_get_ticks         equ 0      ; same as the sys_call_table in global.c
INT_VECTOR_SYS_CALL   equ 0x90

global  get_ticks

bits 32
[section .text]

get_ticks:
        mov     eax, _NR_get_ticks        ; sys call request for OS
        int     INT_VECTOR_SYS_CALL
        ret