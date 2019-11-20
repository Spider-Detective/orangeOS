; attributes of descriptor
DA_32   EQU   4000h     ; 32-bit seg

DA_DPL0 EQU     00h     ; DPL = 0
DA_DPL1 EQU     20h     ; DPL = 1
DA_DPL2 EQU     40h     ; DPL = 2
DA_DPL3 EQU     60h     ; DPL = 3

; code/data seg descriptor
DA_DR      EQU     90h     ; 
DA_DRW     EQU     92h     ;
DA_DRWA    EQU     93h     ; 
DA_C       EQU     98h     ;
DA_CR      EQU     9Ah     ;
DA_CCO     EQU     9Ch     ;
DA_CCOR    EQU     9Eh     ;

; system seg descriptor
DA_LDT          EQU     82h      ;
DA_TaskGate     EQU     85h      ;
DA_386TSS       EQU     89h      ;
DA_386CGate     EQU     8Ch      ;
DA_386IGate     EQU     8Eh      ;
DA_386TGate     EQU     8Fh      ;

SA_TIL     EQU     4     ; position of TI in a selector, on: LDT; off: GDT

; descriptor struct
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