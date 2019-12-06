; attributes of descriptor
DA_32         EQU   4000h     ; 32-bit seg
DA_LIMIT_4K   EQU   8000h     ; segment gradularity for 4K  

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

; SA: Selector Attribute
SA_RPL0    EQU     0
SA_RPL1    EQU     1
SA_RPL2    EQU     2
SA_RPL3    EQU     3     ;

SA_TIG     EQU     0
SA_TIL     EQU     4     ; position of TI in a selector, on: LDT; off: GDT

; Constants for paging mechanism
PG_P       EQU     1     ; page existing property
PG_RWR     EQU     0     ; R/W attr, read/exec
PG_RWW     EQU     2     ; R/W attr, read/write/exec
PG_USS     EQU     0     ; U/S attr, system level
PG_USU     EQU     4     ; U/S attr, user level

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

; gate struct
; Gate Selector, Offset, DCount, Attr
;    Selector: dw
;    Offset:   dd
;    DCount:   db
;    Attr:     db
%macro Gate 4
        dw      (%2 & 0FFFFh)                       ; offset 1
        dw      %1                                  ; selector
        dw      (%3 & 1Fh) | ((%4 << 8) & 0FF00h)   ; attr
        dw      ((%2 >> 16) & 0FFFFh)               ; offset 2
%endmacro   ; 8 bytes