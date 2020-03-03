%include "sconst.inc"

INT_VECTOR_SYS_CALL   equ 0x90
_NR_printx            equ 0      ; same as the sys_call_table in global.c
_NR_sendrec           equ 1

global  printx
global  sendrec

bits 32
[section .text]

; get_ticks:
;         mov     eax, _NR_get_ticks        ; sys call request for OS
;         int     INT_VECTOR_SYS_CALL
;         ret

; write:
;         mov     eax, _NR_write
;         mov     ebx, [esp + 4]
;         mov     ecx, [esp + 8]
;         int     INT_VECTOR_SYS_CALL
;         ret

printx:
		push    edx

        mov     eax, _NR_printx
        mov     edx, [esp + 4 + 4]          
        int     INT_VECTOR_SYS_CALL

		pop     edx

        ret

sendrec:
		push    ebx
		push    ecx
		push    edx

        mov     eax, _NR_sendrec
        mov     ebx, [esp + 12 + 4]          ; function
        mov     ecx, [esp + 12 + 8]          ; src_dest
        mov     edx, [esp + 12 + 12]         ; p_msg
        int     INT_VECTOR_SYS_CALL

		pop     edx
		pop     ecx
		pop     ebx

        ret