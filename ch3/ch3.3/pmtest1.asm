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
LABEL_DESC_PAGE_TBL:   Descriptor   PageTblBase,      4096 * 8 - 1, DA_DRW       ;    Page Tables 
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
; used in real mode:
; strings
_szPMMessage:			        db		"In Protect Mode now.^-^", 0Ah, 0Ah, 0   ; shown in protected mode
_szMemChkTitle:                 db      "BaseAddrL BaseAddrH LengthLow LengthHigh   Type", 0Ah, 0  
_szRAMSize                      db      "RAM size:", 0
_szReturn                       db      0Ah, 0

; variables
_wSPValueInRealMode	            dw	   	  0
_dwMCRNumber:                   dd        0         ; number of memory check results
_dwDispPos:                     dd        (80 * 14 + 0) * 2      ; display position
_dwMemSize:                     dd        0
_ARDStruct:              ; Address Range Descriptor Structure
		_dwBaseAddrLow:         dd        0
		_dwBaseAddrHigh:        dd        0
		_dwLengthLow:           dd        0
		_dwLengthHigh:          dd        0
		_dwType:                dd        0

_MemChkBuf:    times   256      db        0

; used in protect mode (need to offset from seg base addr):
szPMMessage		         equ     _szPMMessage      - $$
szMemChkTitle            equ     _szMemChkTitle    - $$
szRAMSize                equ     _szRAMSize        - $$
szReturn                 equ     _szReturn         - $$
dwDispPos                equ     _dwDispPos        - $$
dwMemSize                equ     _dwMemSize        - $$
dwMCRNumer               equ     _dwMCRNumber      - $$
ARDStruct                equ     _ARDStruct        - $$
		dwBaseAddrLow    equ     _dwBaseAddrLow    - $$
		dwBaseAddrHigh   equ     _dwBaseAddrHigh   - $$
		dwLengthLow      equ     _dwLengthLow      - $$
		dwLengthHigh     equ     _dwLengthHigh     - $$
		dwType           equ     _dwType           - $$
MemChkBuf                equ     _MemChkBuf        - $$

DataLen				     equ	 $ - LABEL_DATA
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
		mov     [_wSPValueInRealMode], sp

		; get the memory size
		mov     ebx, 0
		mov     di, _MemChkBuf
.loop:
        mov     eax, 0E820h
		mov     ecx, 20
		mov     edx, 0534D4150h
		int     15h                   ; prepare regs and do interrupt
		jc      LABEL_MEM_CHK_FAIL    ; error if CF bit is set to 1
		add     di, 20                ; size of each ARDS is 20, saved into _MemChkBuf
		inc     dword [_dwMCRNumber]
		cmp     ebx, 0                ; end loop if ebx is 0
		jne     .loop
		jmp     LABEL_MEM_CHK_OK
LABEL_MEM_CHK_FAIL:
		mov     dword [_dwMCRNumber], 0

LABEL_MEM_CHK_OK:
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

		mov     sp, [_wSPValueInRealMode]

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
		mov		ax, SelectorData
		mov 	es, ax
		mov		ax, SelectorVideo
		mov		gs, ax						; store video seg selector into gs
											; gs's selector points to the index of DESC_VIDEO descriptor in GDT

		mov		ax, SelectorStack
		mov		ss, ax

		mov		esp, TopOfStack     ; changed the ss and esp to make the change in this seg always inside the stack segmentse

		; show a string
		push    szPMMessage
		call    DispStr
		add     esp, 4

		push    szMemChkTitle
		call    DispStr
		add     esp, 4

		call    DispMemSize   ; show memory info

		call    SetupPaging

		jmp		SelectorCode16:0      ; first step of jumping to 16-bit mode

; start paging mechanism -----------------------------------------
; page: continuous chunk of memory, size 4KB
; page table: 1024 * 4 bytes (fit into 1 page)
; page table entry: 1024 entries in each page table, each entry points to a physical page
; page directory: 1024 * 4 bytes (fit into 1 page)
; page directory entry: 1024 entries in each page directory, each entry points to a page table
SetupPaging:

		; calculate how many PDE (or page table) is required according to memory size
		xor     edx, edx
		mov     eax, [dwMemSize]
		mov     ebx, 400000h        ; 400000h = 4M = 1024 (page table entry) * 4096 (size of a physical page), memory size of a page table
		div     ebx
		mov     ecx, eax            ; ecx is the number of page table, or PDE
		test    edx, edx
		jz      .no_remainder
		inc     ecx                 ; add one page table if there is remainder
.no_remainder:
		push    ecx

		; to simplify, all linear address equals to its physical address

		; initiate page directory
		mov     ax, SelectorPageDir        ; front address is PageDirBase 
		mov     es, ax
		mov     ecx, 1024                   ; 1K page directory entries in total
		xor     edi, edi
		xor     eax, eax
		mov     eax, PageTblBase | PG_P | PG_USU | PG_RWW

.1:
		stosd                      ; move edi by 4 each time
		add		eax, 4096          ; for simplicity, all page tables are continuous in memory
		loop    .1

		; initiate all page tables (1Kpage table entries, 4M memory)
		mov     ax, SelectorPageTbl     ; base address PageTblBase
		mov     es, ax
		pop     eax
		mov     ebx, 1024           ; 1024 page table entries for each page table
		mul     ebx
		mov     ecx, eax            ; PTE No. = Page table No. * 1024
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


DispMemSize:
		push    esi
		push    edi
		push    ecx

		mov     esi, MemChkBuf
		mov     ecx, [dwMCRNumer]   ; for (int i = 0; i < [MCRNumber]; i++) // get ARDS each time
.loop:                              ; {
		mov     edx, 5              ;    for (int j = 0; j < 5; j++)  // get a member in ARDS, 5 in total
		mov     edi, ARDStruct      ;    {      // show up the five members
.1: 
        push    dword [esi]
		call    DispInt             ;        DispInt(MemCheckBuf[j * 4]);  // show up the member
		pop     eax      
		stosd                       ;        ARDStruct[j * 4] = MemChkBuf[j * 4];
		add     esi, 4
		dec     edx
		cmp     edx, 0
		jnz     .1                  ;    }
		call    DispReturn          ;    printf("\n");
		cmp     dword [dwType], 1   ;    if (Type == AddressRangeMemory)
		jne     .2                  ;    {
		mov     eax, [dwBaseAddrLow];        // refer to the values defined in ARDStruct
		add     eax, [dwLengthLow]  ;    
		cmp     eax, [dwMemSize]    ;        if (BaseAddrLow + LengthLow > MemSize)
		jb      .2                  ;
		mov     [dwMemSize], eax    ;            MemSize = BaseAddrLow + LengthLow;
.2:                                 ;     }
		loop    .loop               ; }
    
	    call    DispReturn          ; printf("\n");
		push    szRAMSize           ;
		call    DispStr             ; printf("RAM size:");
		add     esp, 4 

		push    dword [dwMemSize]
		call    DispInt             ; DispInt(MemSize);
		add     esp, 4

		pop     ecx                 ; // print out all info for each ARDS,
		pop     edi                 ; // and print out the max memory size needed
		pop     esi
		ret

%include        "lib.inc"           ; lib functions

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