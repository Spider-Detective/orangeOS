; ===========================
; to compile: nasm pmtest1.asm -o pmtest1.com
;
; ===========================

%include	"pm.inc"	; constants, macros

org	0100h
	jmp	LABEL_BEGIN

[SECTION .gdt]
; GDT
;								    base		limit		    	attr
LABEL_GDT:	           Descriptor	     0,		            0, 0	           ; empty descriptor, base of GDT
LABEL_DESC_NORMAL:     Descriptor        0,            0ffffh, DA_DRW       ;
LABEL_DESC_CODE32:     Descriptor	     0,  SegCode32Len - 1, DA_C + DA_32 ;
LABEL_DESC_CODE16:     Descriptor        0,            0ffffh, DA_C         ;
LABEL_DESC_CODE_DEST:  Descriptor        0,SegCodeDestLen - 1, DA_C + DA_32 ;
LABEL_DESC_CODE_RING3: Descriptor        0, SegCodeRing3Len-1, DA_C + DA_32 + DA_DPL3
LABEL_DESC_DATA:       Descriptor        0,       DataLen - 1, DA_DRW       ;
LABEL_DESC_STACK:      Descriptor        0,        TopOfStack, DA_DRWA + DA_32       ;
LABEL_DESC_STACK3:     Descriptor        0,       TopOfStack3, DA_DRWA + DA_32 + DA_DPL3
LABEL_DESC_TEST:       Descriptor 0500000h,            0ffffh, DA_DRW       ;   large addres (5MB)
LABEL_DESC_LDT:        Descriptor        0,        LDTLen - 1, DA_LDT       ; LDT
LABEL_DESC_TSS:        Descriptor        0,        TSSLen - 1, DA_386TSS
LABEL_DESC_VIDEO:      Descriptor  0B8000h,            0ffffh, DA_DRW + DA_DPL3    ; set the privilege to 3 instead of 0

; gates
LABEL_CALL_GATE_TEST: Gate SelectorCodeDest,    0,    0, DA_386CGate + DA_DPL3
; GDT end

GdtLen		equ		$ - LABEL_GDT	; length of GDT
GdtPtr		dw		GdtLen - 1		; limit of GDT
			dd		0				; base of GDT

; GDT selector (basically the offset w.r.t to LABEL_GDT base address)
SelectorNormal		equ		LABEL_DESC_NORMAL		- LABEL_GDT
SelectorCode32		equ		LABEL_DESC_CODE32		- LABEL_GDT
SelectorCode16		equ		LABEL_DESC_CODE16		- LABEL_GDT
SelectorCodeDest    equ     LABEL_DESC_CODE_DEST    - LABEL_GDT
SelectorCodeRing3   equ     LABEL_DESC_CODE_RING3   - LABEL_GDT + SA_RPL3
SelectorData		equ		LABEL_DESC_DATA	     	- LABEL_GDT
SelectorStack		equ		LABEL_DESC_STACK		- LABEL_GDT
SelectorStack3      equ     LABEL_DESC_STACK3       - LABEL_GDT + SA_RPL3
SelectorTest		equ		LABEL_DESC_TEST  		- LABEL_GDT
SelectorLDT         equ     LABEL_DESC_LDT          - LABEL_GDT
SelectorTSS         equ     LABEL_DESC_TSS          - LABEL_GDT
SelectorVideo		equ		LABEL_DESC_VIDEO		- LABEL_GDT

SelectorCallGateTest    equ     LABEL_CALL_GATE_TEST       - LABEL_GDT + SA_RPL3
; END of [SECTION .gdt]

[SECTION .data1]  ; data seg
ALIGN	32
[BITS	32]
LABEL_DATA:
SPValueInRealMode	dw		0
; strings
PMMessage:			db		"In Protect Mode now.", 0   ; shown in protected mode
OffsetPMMessage		equ		PMMessage - $$
StrTest:			db		"AACDEFGHIJKLMNOPQRSTUVWXYZ", 0
OffsetStrTest		equ		StrTest - $$
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

; stack seg ring3
[SECTION .s3]
ALIGN   32
[BITS   32]
LABEL_STACK3:
	    times 512 db 0
TopOfStack3    equ     $ - LABEL_STACK3 - 1
; END of [SECTION .s3]

; TSS
[SECTION .tss]
ALIGN   32
[BITS   32]
LABEL_TSS:  ; see Figure 3.20, write from byte 0 to 100
; DD: write double word (32 bits), DW: 16bits
		      DD     0                       ; Back
			  DD     TopOfStack              ; stack for ring0
			  DD     SelectorStack           ; 
			  DD     0                       ; stack for ring 1
			  DD     0                       ; 
			  DD     0                       ; stack for ring 2
			  DD     0                       ; 
			  DD     0                       ; GR3
			  DD     0                       ; EIP
			  DD     0                       ; EFLAGS
			  DD     0                       ; EAX
			  DD     0                       ; ECX
			  DD     0                       ; EDX
			  DD     0                       ; EBX
			  DD     0                       ; ESP
			  DD     0                       ; EBP
			  DD     0                       ; ESI
			  DD     0                       ; EDI
			  DD     0                       ; ES
			  DD     0                       ; CS
			  DD     0                       ; SS
			  DD     0                       ; DS
			  DD     0                       ; FS
			  DD     0                       ; GS
			  DD     0                       ; LDT
			  DW     0                       ; trap
			  DW     $ - LABEL_TSS + 2       ; Base addr of I/O bitmap
			  DB     0ffh                    ; Ending sign of I/O bitmap

TSSLen        equ    $ - LABEL_TSS
; END of [SECTION .tss]

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

		; initialize call gates seg descriptor
		xor		eax, eax
		mov		ax, cs
		shl		eax, 4
		add		eax, LABEL_SEG_CODE_DEST
		mov		word [LABEL_DESC_CODE_DEST + 2], ax  ; need to update the base, since it is 0 when initialized
		shr		eax, 16
		mov		byte [LABEL_DESC_CODE_DEST + 4], al
		mov		byte [LABEL_DESC_CODE_DEST + 7], ah

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

		; initialize stack code segment descriptor for privilege 3
		xor		eax, eax
		mov		ax, ds
		shl		eax, 4
		add		eax, LABEL_STACK3
		mov		word [LABEL_DESC_STACK3 + 2], ax  ; need to update the base, since it is 0 when initialized
		shr		eax, 16
		mov		byte [LABEL_DESC_STACK3 + 4], al
		mov		byte [LABEL_DESC_STACK3 + 7], ah

		; initialize LDT descriptor
		xor		eax, eax
		mov		ax, ds
		shl		eax, 4
		add		eax, LABEL_LDT
		mov		word [LABEL_DESC_LDT + 2], ax  ; need to update the base, since it is 0 when initialized
		shr		eax, 16
		mov		byte [LABEL_DESC_LDT + 4], al
		mov		byte [LABEL_DESC_LDT + 7], ah

		; initialize LDT_CODEA descriptor
		xor		eax, eax
		mov		ax, ds
		shl		eax, 4
		add		eax, LABEL_CODE_A
		mov		word [LABEL_LDT_DESC_CODEA + 2], ax  ; need to update the base, since it is 0 when initialized
		shr		eax, 16
		mov		byte [LABEL_LDT_DESC_CODEA + 4], al
		mov		byte [LABEL_LDT_DESC_CODEA + 7], ah

		; initialize Ring3 descriptor
		xor		eax, eax
		mov		ax, ds
		shl		eax, 4
		add		eax, LABEL_CODE_RING3
		mov		word [LABEL_DESC_CODE_RING3 + 2], ax  ; need to update the base, since it is 0 when initialized
		shr		eax, 16
		mov		byte [LABEL_DESC_CODE_RING3 + 4], al
		mov		byte [LABEL_DESC_CODE_RING3 + 7], ah

		; initialize TSS descriptor
		xor		eax, eax
		mov		ax, ds
		shl		eax, 4
		add		eax, LABEL_TSS
		mov		word [LABEL_DESC_TSS + 2], ax  ; need to update the base, since it is 0 when initialized
		shr		eax, 16
		mov		byte [LABEL_DESC_TSS + 4], al
		mov		byte [LABEL_DESC_TSS + 7], ah

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
		mov		ax, SelectorData
		mov 	ds, ax
		;mov 	ax, SelectorTest
		;mov 	es, ax
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
		; mov		al, 'P'
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

		call	DispReturn

		; call	TestRead
		; call	TestWrite
		; call	TestRead

		; jmp		SelectorCode16:0      ; first step of jumping to 16-bit mode

		mov     ax, SelectorTSS
		ltr     ax                  ; load TSS before changing privilege

		; enter privilege 3 mode
		push    SelectorStack3    ; push ss for privilege 3
		push    TopOfStack3       ; push esp
		push    SelectorCodeRing3 ; push cs
		push    0                 ; push eip
		retf      ; jump from privilege 0 to 3!
 		
		ud2       ; bug test: should never arrive here

		; test call gates by printing char 'C'
		call    SelectorCallGateTest:0    
		;call    SelectorCodeDest:0   ; this also works, without using call gates, but can only be used with no privilege change
		
		; load LDT
		mov     ax, SelectorLDT
		lldt    ax

		jmp     SelectorLDTCodeA:0    ; jump into local task

; -------------------------------
TestRead:
		xor		esi, esi
		mov		ecx, 8
.loop:
		mov		al, [es:esi]
		call	DispAL
		inc		esi
		loop	.loop

		call	DispReturn

		ret
; end of TestRead

TestWrite:
		push	esi
		push	edi
		xor		esi, esi
		xor		edi, edi
		mov		esi, OffsetStrTest
		cld
.1:
		lodsb
		test	al, al
		jz		.2
		mov		[es:edi], al
		inc		edi
		jmp		.1
.2:
		pop 	edi
		pop		esi

		ret
; end of TestWrite

; show the digit in AL, 
; input: digit inside AL, edi is pointing the position of the next char
; registers to be changed in the function: ax, edi
DispAL:
		push	ecx
		push	edx

		mov		ah, 0Ch
		mov		dl, al
		shr		al, 4
		mov		ecx, 2
.begin:
		and		al, 01111b
		cmp     al, 9
		ja      .1
		add     al, '0'
		jmp     .2
.1:
        sub     al, 0Ah
		add     al, 'A'
.2:
        mov     [gs:edi], ax
		add     edi, 2

		mov     al, dl
		loop    .begin
		add     edi, 2

		pop     edx
		pop     ecx

		ret
; end of DispAL

DispReturn:
		push    eax
		push    ebx
		mov     eax, edi
		mov     bl, 160
		div     bl
		and     eax, 0FFh
		inc     eax
		mov     bl, 160
		mul     bl
		mov     edi, eax
		pop     ebx
		pop     eax

		ret

SegCode32Len		equ		$ - LABEL_SEG_CODE32
; END of [SECTION .s32]

[SECTION .sdest]  ; target seg of call gates
[BITS   32]

LABEL_SEG_CODE_DEST:
		;jmp    $
		mov     ax, SelectorVideo
		mov     gs, ax

		mov     edi, (80 * 12 + 0) * 2
		mov     ah, 0Ch   
		mov     al, 'C'
		mov     [gs:edi], ax

		; load LDT here, instead of calling LDTCodeA in p.s32 as before
		mov     ax, SelectorLDT
		lldt    ax

        jmp     SelectorLDTCodeA:0     ; jump into LDT, print 'L'

		;retf   ; the code will execute right after the call command

SegCodeDestLen  equ    $ - LABEL_SEG_CODE_DEST
; END of [SECTION .sdest]

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
		and     al, 11111110b
		mov     cr0, eax

LABEL_GO_BACK_TO_REAL:
; this will jump to the section of LABEL_BEGIN and LABEL_REAL_ENTRY, with the cs set in LABEL_BEGIN (real-mode cs)
        jmp     0:LABEL_REAL_ENTRY   ; the segment address (currently 0) is set in LABEL_BEGIN seg, at the beginning of the code

Code16Len       equ      $ - LABEL_SEG_CODE16

; end of [SECTION .s16code]

; LDT
[SECTION .ldt]
ALIGN     32
LABEL_LDT:
LABEL_LDT_DESC_CODEA: Descriptor 0, CodeALen - 1, DA_C + DA_32

LDTLen      equ   $ - LABEL_LDT

; LDT selector (use SA_TIL on)
SelectorLDTCodeA       equ LABEL_LDT_DESC_CODEA - LABEL_LDT + SA_TIL
; END of [SECTION .ldt]

; CodeA (LDT, 32-bit seg)
[SECTION .la]
ALIGN     32
[BITS     32]
LABEL_CODE_A:
    mov    ax, SelectorVideo
	mov    gs, ax

	mov    edi, (80 * 13 + 0) * 2
	mov    ah, 0Ch
	mov    al, 'L'
	mov    [gs:edi], ax

	jmp    SelectorCode16:0
CodeALen    equ   $ - LABEL_CODE_A
; END of [SECTION .la]

; CodeRing3, write a '3' into the screen
[SECTION .ring3]
ALIGN    32
[BITS    32]
LABEL_CODE_RING3:
		mov		ax, SelectorVideo
		mov     gs, ax

		mov     edi, (80 * 14 + 0) * 2
		mov     ah, 0Ch
		mov     al, '3'
		mov     [gs:edi], ax

        call    SelectorCallGateTest:0  ; jump from privilege 3 to 0! by using call gates

		jmp     $  ; stay here for now
SegCodeRing3Len  equ      $ - LABEL_CODE_RING3
; END of [SECTION .ring3]
