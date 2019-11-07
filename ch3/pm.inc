; type of descriptor
DA_32   EQU   4000h     ; 32-bit seg

; store the type of seg descriptor
DA_DRW  EQU     92h     ;
DA_C    EQU     98h     ; 

; descriptor
; usage: Descriptor Base, Limit, Attr
; Base: dd
; Limit: dd (low 20 bits available)
; Attr: dw (lower 4 bits of higher byte are always 0)

%macro Descriptor 3
        dw      %2 & 0FFFFh     ; segment limit 1
        dw      %1 & 0FFFFh     ; segment base 1
        db      (%1 >> 16) & 0FFh   ; base 2
        dw      ((%2 >> 8) & 0F00h) | (%3 & 0F0FFh) ; attr 1 + limit 2 + attr 2
        db      (%1 >> 24) & 0FFh  ; base 3
%endmacro ; 8 bytes