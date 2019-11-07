; to compile: nasm pmtest1.asm -o pmtest1.bin

%include	"pm.inc"	; constants, macros

org	07c00h
	jmp	LABEL_BEGIN

[SECTION .gdt]
; GDT
;								base		limit			attr
LABEL_GDT:	       Descriptor	    0,		           0, 0	           ; empty descriptor, base of GDT
LABEL_DESC_CODE32: Descriptor	    0,  SegCode32Len - 1, DA_C + DA_32 ;
LABEL_DESC_VIDEO:  Descriptor 0B8000h,            0ffffh, DA_DRW       ;
; GDT end

GdtLen		equ		$ - LABEL_GDT	; length of GDT
GdtPtr		dw		GdtLen - 1		; limit of GDT
			dd		0				; base of GDT

; GDT selector (basically the offset w.r.t to LABEL_GDT base address)
SelectorCode32		equ		LABEL_DESC_CODE32		- LABEL_GDT
SelectorVideo		equ		LABEL_DESC_VIDEO		- LABEL_GDT
; END of [SECTION .gdt]

[SECTION .s16]
[BITS 	16]
LABEL_BEGIN:
		mov		ax, cs
		mov		ds, ax
		mov		es, ax
		mov		ss, ax
		mov		sp, 0100h

		; initialize 32-bit code segment descriptor
		xor		eax, eax
		mov		ax, cs
		shl		eax, 4
		add		eax, LABEL_SEG_CODE32
		mov		word [LABEL_DESC_CODE32 + 2], ax  ; need to update the base, since it is 0 when initialized
		shr		eax, 16
		mov		byte [LABEL_DESC_CODE32 + 4], al
		mov		byte [LABEL_DESC_CODE32 + 7], ah

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

; END of [SECTION .s16]

[SECTION .s32] ; 32 bit code segment, jumped from real mode
[BITS 	32]

LABEL_SEG_CODE32:
		mov		ax, SelectorVideo
		mov		gs, ax						; store video seg selector into gs
											; gs's selector points to the index of DESC_VIDEO descriptor in GDT
		mov		edi, (80 * 11 + 79) * 2     ; line 11, col 79 on screen
		mov		ah, 0Ch						; 0000: black back, 1100: red char
		mov		al, 'P'
		mov		[gs:edi], ax				; write ax (pixel) into gs (DESC_VIDEO) with offset edi (pixel pos)

		jmp 	$

SegCode32Len		equ		$ - LABEL_SEG_CODE32
; END of [SECTION .s32]
