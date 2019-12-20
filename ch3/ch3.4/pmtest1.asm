; ===========================
; to compile: nasm pmtest1.asm -o pmtest1.com
;
; ===========================

%include	"pm.inc"	; constants, macros

; define two set of page directory
PageDirBase0			equ		200000h  ; starting address of page directory: 2M
PageTblBase0            equ     201000h  ; starting address of page table: 2M+4K 
PageDirBase1	        equ     210000h  ; 2M + 64K
PageTblBase1            equ     211000h  ; 2M + 64K + 4K

LinearAddrDemo   equ     00401000h  ; PDE 1, PTE 1, offset 0 
ProcFoo          equ     00401000h  ; first call from LinearAddrDemo, same addr as LinearAddrDemo
ProcBar          equ     00501000h  ; second call, mapped address from LinearAddrDemo
ProcPagingDemo   equ     00301000h

org	0100h
	jmp	LABEL_BEGIN

[SECTION .gdt]
; GDT
;								       base		         limit		    	attr
LABEL_GDT:	           Descriptor	          0,		         0, 0	           ; empty descriptor, base of GDT
LABEL_DESC_NORMAL:     Descriptor             0,            0ffffh, DA_DRW       ;
LABEL_DESC_FLAT_C:     Descriptor             0,           0fffffh, DA_CR|DA_32|DA_LIMIT_4K ; 0~4G, notice DA_LIMIT_4K
LABEL_DESC_FLAT_RW:    Descriptor             0,           0fffffh, DA_DRW|DA_LIMIT_4K   ; same as FLAT_C, only difference is attr
LABEL_DESC_CODE32:     Descriptor	          0,  SegCode32Len - 1, DA_CR|DA_32 ;
LABEL_DESC_CODE16:     Descriptor             0,            0ffffh, DA_C         ;
LABEL_DESC_DATA:       Descriptor             0,       DataLen - 1, DA_DRW       ;
LABEL_DESC_STACK:      Descriptor             0,        TopOfStack, DA_DRWA|DA_32       ;
LABEL_DESC_VIDEO:      Descriptor       0B8000h,            0ffffh, DA_DRW    ; set the privilege to 3 instead of 0

GdtLen		equ		$ - LABEL_GDT	; length of GDT
GdtPtr		dw		GdtLen - 1		; limit of GDT
			dd		0				; base of GDT

; GDT selector (basically the offset w.r.t to LABEL_GDT base address)
SelectorNormal		equ		LABEL_DESC_NORMAL		- LABEL_GDT
SelectorFlatC		equ		LABEL_DESC_FLAT_C		- LABEL_GDT
SelectorFlatRW		equ		LABEL_DESC_FLAT_RW   	- LABEL_GDT
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
_dwDispPos:                     dd        (80 * 6 + 0) * 2      ; display position
_dwMemSize:                     dd        0
_ARDStruct:              ; Address Range Descriptor Structure
		_dwBaseAddrLow:         dd        0
		_dwBaseAddrHigh:        dd        0
		_dwLengthLow:           dd        0
		_dwLengthHigh:          dd        0
		_dwType:                dd        0
_PageTableNumber                dd        0
_SavedIDTR:                     dd        0      ; save IDTR
								dd        0
_SavedIMREG:                    db        0      ; save value of IMR
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
SavedIDTR                equ     _SavedIDTR        - $$
SavedIMREG               equ     _SavedIMREG       - $$
PageTableNumber          equ     _PageTableNumber  - $$

DataLen				     equ	 $ - LABEL_DATA
; END of [SECTION .data1]

; IDT
[SECTION .idt]
ALIGN    32
[BITS    32]
LABEL_IDT:
; gate                 target selector         offset    DCount   Attribute
%rep 32   
			Gate        SelectorCode32, SpuriousHandler,     0,  DA_386IGate
%endrep
.020h:	    Gate        SelectorCode32,    ClockHandler,     0,  DA_386IGate  ; for interrupt vec 020h
%rep 95 
			Gate        SelectorCode32, SpuriousHandler,     0,  DA_386IGate
%endrep
.080h:	    Gate        SelectorCode32,  UserIntHandler,     0,  DA_386IGate  ; for interrupt vec 080h 

IdtLen      equ         $ - LABEL_IDT
IdtPtr      dw          IdtLen - 1        ; seg limit
			dd          0                 ; seg base addr
; END of [SECTION .idt]

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

		; prepare for loading LDTR
		xor     eax, eax
		mov     ax, ds
		shl     eax, 4
		add     eax, LABEL_IDT
		mov     dword [IdtPtr + 2], eax  ; give the idt base addr

		; save IDTR
		sidt    [_SavedIDTR]

		; save IMR
		in      al, 21h
		mov     [_SavedIMREG], al

		; load GDTR
	    lgdt	[GdtPtr]

		cli  ; different handling for interrupt in protected mode

		; load IDTR
		lidt    [IdtPtr]

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

		lidt    [_SavedIDTR]     ; recover IDT value
		mov     al, [_SavedIMREG]   
		out     21h, al          ; recover IMR

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
		mov 	ds, ax                      ; give the addr for data seg
		mov 	es, ax
		mov		ax, SelectorVideo
		mov		gs, ax						; store video seg selector into gs
											; gs's selector points to the index of DESC_VIDEO descriptor in GDT

		mov		ax, SelectorStack           ; give the addr for stack seg
		mov		ss, ax
		mov		esp, TopOfStack     ; changed the ss and esp to make the change in this seg always inside the stack segmentse

		; enable 8259 and try an interrupt
		call    Init8259A
		int     080h
		sti                   ; set IF
		jmp     $             ; enter infinite loop to wait for timer interrupt

		; show a string
		push    szPMMessage
		call    DispStr
		add     esp, 4

		push    szMemChkTitle
		call    DispStr
		add     esp, 4

		call    DispMemSize   ; show memory info

		call    PagingDemo    ; entry point of the paging experiment code

		call    SetRealmode8295A

		jmp		SelectorCode16:0      ; first step of jumping to 16-bit mode

Init8259A:   ; see Figure 3.40
; ICW: Initialization Command Word
; OCW: Operation Control Word
		mov     al, 011h
		out     020h, al       ; ICW1, to master 8259A
		call    io_delay

		out     0A0h, al       ; ICW1, to slave 8295A
		call    io_delay

		mov     al, 020h       ; interrupt vector 020h -> IRQ0, IRQ1-7 will be 021h-027h
		out     021h, al       ; ICW2, to master 8259A
		call    io_delay

		mov     al, 028h       ; interrupt vector 028h -> IRQ8, IRQ9-15 will be 29h-2Fh
		out     0A1h, al       ; ICW2, to slave 8259A
		call    io_delay

		mov     al, 004h       ; claim master's IR23 connects to slave 8259A
		out     021h, al       ; ICW3, to master 8259A
		call    io_delay

		mov     al, 002h       ; claim slave connects to master's IR2
		out     0A1h, al       ; ICW3, to slave
		call    io_delay

		mov     al, 001h  
		out     021h, al       ; ICW4, to master
		call    io_delay

		out     0A1h, al       ; ICW4, to slave
		call    io_delay

		mov     al, 11111110b  ; only enable timer's interrupt on master
		out     021h, al       ; OCW1, master
		call    io_delay

		mov     al, 11111111b  ; disable all interrupt on slave
		out     0A1h, al       ; OCW1, slave
		call    io_delay

		ret

SetRealmode8295A:
		mov     ax, SelectorData
		mov     fs, ax

		mov     al, 017h
		out     020h, al       ; ICW1, master
		call    io_delay

		mov     al, 008h       ; IRQ0 -> 0x8
		out     021h, al       ; ICW2, master
		call    io_delay

		mov     al, 001h       
		out     021h, al       ; ICW4, master
		call    io_delay

		mov     al, [fs:SavedIMREG]
		out     021h, al       ; recover IMR
		call    io_delay

		ret

io_delay:
		nop     ; do thing, just move to next instruction
		nop
		nop
		nop
		ret

_ClockHandler: 
ClockHandler       equ         _ClockHandler - $$
		inc     byte [gs:((80 * 0 + 70) * 2)]  ; increment the char shown on screen
		mov     al, 20h
		out     20h, al        ; send EOI to 20h to continue to receive and handle interrupt    
		iretd

_UserIntHandler:
UserIntHandler     equ         _UserIntHandler - $$
		mov     ah, 0Ch
		mov     al, 'I'
		mov     [gs:((80 * 0 + 70) * 2)], ax     
		iretd

_SpuriousHandler:   ; draw red ! on screen
SpuriousHandler    equ         _SpuriousHandler - $$
		mov     ah, 0Ch
		mov     al, '!'
		mov     [gs:((80 * 0 + 75) * 2)], ax    
		jmp     $ 
		iretd

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
		mov     ecx, eax            ; ecx is the number of page tables, or PDE
		test    edx, edx
		jz      .no_remainder
		inc     ecx                 ; add one page table if there is remainder
.no_remainder:
		mov     [PageTableNumber], ecx     ; store the number of page tables

		; to simplify, all linear address equals to its physical address

		; initiate page directory
		mov     ax, SelectorFlatRW         
		mov     es, ax
		mov     edi, PageDirBase0         ; base address is PageDirBase0
		xor     eax, eax
		mov     eax, PageTblBase0 | PG_P | PG_USU | PG_RWW
.1:
		stosd                      ; move edi by 4 each time, initialize one PDE
		add		eax, 4096          ; for simplicity, all page tables are continuous in memory
		loop    .1

		; initiate all page tables (1Kpage table entries, 4M memory)
		; still use the same seg SelectorFlatRW for es
		mov     eax, [PageTableNumber]     ; get the number of page tables
		mov     ebx, 1024           ; 1024 page table entries for each page table
		mul     ebx
		mov     ecx, eax            ; PTE No. = Page table No. * 1024
		mov     edi, PageTblBase0   ; base address is PageTblBase0
		xor     eax, eax
		mov     eax, PG_P | PG_USU | PG_RWW
.2:
		stosd                   ; initialize one PTE
		add     eax, 4096       ; each page points to 4K space
		loop    .2

		; start paging mechanism
		mov     eax, PageDirBase0
		mov     cr3, eax        ; give the PD base address to cr3
		mov     eax, cr0
		or      eax, 80000000h
		mov     cr0, eax        ; set bit PG for cr0
		jmp     short .3
.3:
		nop

		ret	
; finished starting paging mechanism --------------------------------------

; page mechanism experiment
PagingDemo:
		; prepare for MemCpy: dest seg: es or SelectorFlatRW, src seg: ds or cs
		mov     ax, cs
		mov     ds, ax
		mov     ax, SelectorFlatRW
		mov     es, ax

		; copy the functions: PagingDemoProc, foo and bar into seg SelectorFlatRW
		push    LenFoo
		push    OffsetFoo
		push    ProcFoo
		call    MemCpy
		add     esp, 12

		push    LenBar
		push    OffsetBar
		push    ProcBar
		call    MemCpy
		add     esp, 12

		push    LenPagingDemoAll
		push    OffsetPagingDemoProc
		push    ProcPagingDemo
		call    MemCpy
		add     esp, 12

		mov     ax, SelectorData  ; recover es and ds
		mov     ds, ax
		mov     es, ax

		call    SetupPaging       

		call    SelectorFlatC:ProcPagingDemo  ; should call foo
		call    PSwitch                       ; page switch to the second page dir
		call    SelectorFlatC:ProcPagingDemo  ; should call bar

		ret

; function to switch paging directory
; change from PageDirBase0 to PageDirBase1
PSwitch:
		; initiate page directory
		mov     ax, SelectorFlatRW         
		mov     es, ax
		mov     edi, PageDirBase1         ; base address is PageDirBase1
		xor     eax, eax
		mov     eax, PageTblBase1 | PG_P | PG_USU | PG_RWW
		mov     ecx, [PageTableNumber]
.1:
		stosd                      ; move edi by 4 each time, initialize one PDE
		add		eax, 4096          ; for simplicity, all page tables are continuous in memory
		loop    .1

		; initiate all page tables (1Kpage table entries, 4M memory)
		; still use the same seg SelectorFlatRW for es
		mov     eax, [PageTableNumber]     ; get the number of page tables
		mov     ebx, 1024           ; 1024 page table entries for each page table
		mul     ebx
		mov     ecx, eax            ; PTE No. = Page table No. * 1024
		mov     edi, PageTblBase1   ; base address is PageTblBase1
		xor     eax, eax
		mov     eax, PG_P | PG_USU | PG_RWW
.2:
		stosd                   ; initialize one PTE
		add     eax, 4096       ; each page points to 4K space
		loop    .2

		; map the LinearAddrDemo (called by PagingDemoProc below) to ProcBar:
		; LinearAddrDemo: linear address -> ProcBar: physical address
		; see Figure 3.27
		; first consider LinearAddrDemo as a linear address, and 
		; get the corresponding PTE address from this linear address
		; then assign the value to be the address of ProcBar, using es:eax, the logical address
		mov     eax, LinearAddrDemo
		shr     eax, 22         ; get the highest 10 bits
		mov     ebx, 4096
		mul     ebx
		mov     ecx, eax        ; ecx is the offset of PD
		mov     eax, LinearAddrDemo
		shr     eax, 12
		and     eax, 03FFh      ; 10 bits, get the middle 10 bits
		mov     ebx, 4
		mul     ebx             ; eax now is the offset of PT
		add     eax, ecx    
		add     eax, PageTblBase1
		mov     dword [es:eax], ProcBar | PG_P | PG_USU | PG_RWW ; assign to logical address

		; start paging mechanism
		mov     eax, PageDirBase1
		mov     cr3, eax        ; give the new PD base address to cr3
		jmp     short .3
.3:
		nop

		ret	
; ---------------------------------------------------

; test functions, will be put into the SelectorFlat seg
PagingDemoProc:
OffsetPagingDemoProc      equ      PagingDemoProc - $$
		mov     eax, LinearAddrDemo    ; call the linear address in SelectorFlat seg
		call    eax                    ; short call
		retf
LenPagingDemoAll          equ      $ - PagingDemoProc

foo:
OffsetFoo                 equ      foo - $$
		mov     ah, 0Ch
		mov     al, 'F'
		mov     [gs:((80 * 17 + 0) * 2)], ax
		mov     al, 'o'
		mov     [gs:((80 * 17 + 1) * 2)], ax
		mov     [gs:((80 * 17 + 2) * 2)], ax
		ret
LenFoo                    equ      $ - foo

bar:
OffsetBar                 equ      bar - $$
		mov     ah, 0Ch
		mov     al, 'B'
		mov     [gs:((80 * 18 + 0) * 2)], ax
		mov     al, 'a'
		mov     [gs:((80 * 18 + 1) * 2)], ax
		mov     al, 'r'
		mov     [gs:((80 * 18 + 2) * 2)], ax
		ret
LenBar                    equ      $ - bar
; ---------------------------

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