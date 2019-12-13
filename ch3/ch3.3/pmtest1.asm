; ===========================
; to compile: nasm pmtest1.asm -o pmtest1.com
;
; ===========================

%include	"pm.inc"	; constants, macros

PageDirBase			equ		200000h  ; starting address of page category: 2M
PageTblBase         equ     201000h  ; starting address of page table: 2M+4K 

org	0100h
	jmp	LABEL_BEGIN

[SECTION .gdt]
; GDT
;								       base		         limit		    	attr
LABEL_GDT:	           Descriptor	          0,		         0, 0	           ; empty descriptor, base of GDT
LABEL_DESC_NORMAL:     Descriptor             0,            0ffffh, DA_DRW       ;
LABEL_DESC_PAGE_DIR:   Descriptor   PageDirBase,              4095, DA_DRW        ; Page Directory
LABEL_DESC_PAGE_TBL:   Descriptor   PageTblBase,              1023, DA_DRW|DA_LIMIT_4K;    Page Tables 
LABEL_DESC_CODE32:     Descriptor	          0,  SegCode32Len - 1, DA_C + DA_32 ;
LABEL_DESC_CODE16:     Descriptor             0,            0ffffh, DA_C         ;
LABEL_DESC_DATA:       Descriptor             0,       DataLen - 1, DA_DRW       ;
LABEL_DESC_STACK:      Descriptor             0,        TopOfStack, DA_DRWA + DA_32       ;
LABEL_DESC_VIDEO:      Descriptor       0B8000h,            0ffffh, DA_DRW    ; set the privilege to 3 instead of 0

GdtLen		equ		$ - LABEL_GDT	; length of GDT
GdtPtr		dw		GdtLen - 1		; limit of GDT
			dd		0				; base of GDT

; GDT selector (basically the offset w.r.t to LABEL_GDT base address)
SelectorNormal		equ		LABEL_DESC_NORMAL		- LABEL_GDT
SelectorPageDir		equ		LABEL_DESC_PAGE_DIR		- LABEL_GDT
SelectorPageTbl		equ		LABEL_DESC_PAGE_TBL   	- LABEL_GDT
SelectorCode32		equ		LABEL_DESC_CODE32		- LABEL_GDT
SelectorCode16		equ		LABEL_DESC_CODE16		- LABEL_GDT
SelectorData		equ		LABEL_DESC_DATA	     	- LABEL_GDT
SelectorStack		equ		LABEL_DESC_STACK		- LABEL_GDT
SelectorVideo		equ		LABEL_DESC_VIDEO		- LABEL_GDT
; END of [SECTION .gdt]

[SECTION .data1]  ; data seg
ALIGN	32
[BITS	32]
LABEL_DATA:
SPValueInRealMode	dw		0
; strings
PMMessage:			db		"In Protect Mode now.", 0   ; shown in protected mode
OffsetPMMessage		equ		PMMessage - $$
DataLen				equ		$ - LABEL_DATA
; END of [SECTION .data	1]


; global stack seg
[SECTION .gs]
ALIGN	32
[BITS	32]
LABEL_STACK:	
		times 512 db 0

TopOfStack			equ		$ - LABEL_STACK - 1

; END of [SECTION .gs]

[SECTION .s16]
[BITS 	16]
LABEL_BEGIN:
		mov		ax, cs
		mov		ds, ax
		mov		es, ax
		mov		ss, ax
		mov		sp, 0100h

		mov     [LABEL_GO_BACK_TO_REAL+3], ax  ; save the cs value into LABEL_GO_BACK_TO_REAL's segment position
		mov     [SPValueInRealMode], sp

		; initialize 16-bit code sqgment descriptor
		mov		ax, cs
		movzx   eax, ax
		shl		eax, 4
		add		eax, LABEL_SEG_CODE16
		mov		word [LABEL_DESC_CODE16 + 2], ax  ; need to update the base, since it is 0 when initialized
		shr		eax, 16
		mov		byte [LABEL_DESC_CODE16 + 4], al
		mov		byte [LABEL_DESC_CODE16 + 7], ah

		; initialize 32-bit code segment descriptor
		xor		eax, eax
		mov		ax, cs
		shl		eax, 4
		add		eax, LABEL_SEG_CODE32
		mov		word [LABEL_DESC_CODE32 + 2], ax  ; need to update the base, since it is 0 when initialized
		shr		eax, 16
		mov		byte [LABEL_DESC_CODE32 + 4], al
		mov		byte [LABEL_DESC_CODE32 + 7], ah

		; initialize data code segment descriptor
		xor		eax, eax
		mov		ax, ds
		shl		eax, 4
		add		eax, LABEL_DATA
		mov		word [LABEL_DESC_DATA + 2], ax  ; need to update the base, since it is 0 when initialized
		shr		eax, 16
		mov		byte [LABEL_DESC_DATA + 4], al
		mov		byte [LABEL_DESC_DATA + 7], ah

		; initialize stack code segment descriptor
		xor		eax, eax
		mov		ax, ds
		shl		eax, 4
		add		eax, LABEL_STACK
		mov		word [LABEL_DESC_STACK + 2], ax  ; need to update the base, since it is 0 when initialized
		shr		eax, 16
		mov		byte [LABEL_DESC_STACK + 4], al
		mov		byte [LABEL_DESC_STACK + 7], ah

		; prepare for loading GDTR
		xor 	eax, eax
		mov		ax, ds
		shl		eax, 4
		add		eax, LABEL_GDT		     ; eax <- gdt base
		mov		dword [GdtPtr + 2], eax  ; [GdtPtr + 2] <- gdt base

		; load GDTR
	    lgdt	[GdtPtr]

		cli  ; different handling for interrupt in protected mode

		; open address line A20
		in		al, 92h
		or		al, 00000010b
		out		92h, al

		; ready for switching to protected mode, first bit in cr0 indicates real/protected mode
		mov		eax, cr0
		or		eax, 1
		mov		cr0, eax

		; entering protected mode
		jmp		dword SelectorCode32:0 ; put SelectorCode32 into cs,then jump to Code32Selector:0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

LABEL_REAL_ENTRY:         ; back to here after jump from protected-mode to real-mode
        mov     ax, cs
		mov     ds, ax
		mov     es, ax
		mov     ss, ax

		mov     sp, [SPValueInRealMode]

		; close address line A20
		in      al, 92h     
		and     al, 11111101b
		out     92h, al

		sti

		mov     ax, 4c00h
		int     21h
; END of [SECTION .s16]

[SECTION .s32] ; 32 bit code segment, jumped from real mode
[BITS 	32]

LABEL_SEG_CODE32:
		call    SetupPaging

		mov		ax, SelectorData
		mov 	ds, ax
		mov		ax, SelectorVideo
		mov		gs, ax						; store video seg selector into gs
											; gs's selector points to the index of DESC_VIDEO descriptor in GDT

		mov		ax, SelectorStack
		mov		ss, ax

		mov		esp, TopOfStack     ; changed the ss and esp to make the change in this seg always inside the stack segmentse

		; mov		edi, (80 * 11 + 79) * 2     ; line 11, col 79 on screen
		mov		ah, 0Ch						; 0000: black back, 1100: red char
		xor		esi, esi
		xor 	edi, edi
		mov		esi, OffsetPMMessage
		mov		edi, (80 * 10 + 0) * 2
		cld
.1:
		lodsb
		test 	al, al
		jz		.2
		mov		[gs:edi], ax				; write ax (pixel) into gs (DESC_VIDEO) with offset edi (pixel pos)
		add		edi, 2
		jmp 	.1
.2:		; finished display

		jmp		SelectorCode16:0      ; first step of jumping to 16-bit mode

; start paging mechanism -----------------------------------------
SetupPaging:
		; to simplify, all linear address equals to its physical address

		; initiate page directory
		mov     ax, SelectorPageDir        ; front address is PageDirBase 
		mov     es, ax
		mov     ecx, 1024                   ; 1K table entries in total
		xor     edi, edi
		xor     eax, eax
		mov     eax, PageTblBase | PG_P | PG_USU | PG_RWW

.1:
		stosd                      ; move edi by 4 each time
		add		eax, 4096          ; for simplicity, all page tables are continuous in memory
		loop    .1

		; re-initiate all page tables (1Kpage table entries, 4M memory)
		mov     ax, SelectorPageTbl     ; base address PageTblBase
		mov     es, ax
		mov     ecx, 1024 * 1024      ; 1M page table entries
		xor     edi, edi
		xor     eax, eax
		mov     eax, PG_P | PG_USU | PG_RWW
.2:
		stosd
		add     eax, 4096       ; each page points to 4K space
		loop    .2

		; start paging mechanism
		mov     eax, PageDirBase
		mov     cr3, eax
		mov     eax, cr0
		or      eax, 80000000h
		mov     cr0, eax        ; set bit PG for cr0
		jmp     short .3
.3:
		nop

		ret	
; finished starting paging mechanism --------------------------------------

SegCode32Len      equ         $ - LABEL_SEG_CODE32

; END of [SECTION .s32]


; 16-bit segment, jumped from 32-bit, enter real-mode after jump our
[SECTION .s16code]
ALIGN    32
[BITS    16]
LABEL_SEG_CODE16:
        ; jump back to real-mode
		mov     ax, SelectorNormal   ; only able to return to real-mode from 16-bit seg, so need to first go to a 16-bit seg (SelectorNormal)
		mov     ds, ax
		mov     es, ax
		mov     fs, ax
		mov     gs, ax
		mov     ss, ax

		mov     eax, cr0
		and     eax, 7FFFFFFEh    ; PE=0, PG=0 
		mov     cr0, eax

LABEL_GO_BACK_TO_REAL:
; this will jump to the section of LABEL_BEGIN and LABEL_REAL_ENTRY, with the cs set in LABEL_BEGIN (real-mode cs)
        jmp     0:LABEL_REAL_ENTRY   ; the segment address (currently 0) is set in LABEL_BEGIN seg, at the beginning of the code

Code16Len       equ      $ - LABEL_SEG_CODE16

; end of [SECTION .s16code]