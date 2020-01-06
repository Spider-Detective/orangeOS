
org  07c00h			; Bios will load Boot Sector to 0:7C00 and start execute

; ----------------- constants definitions ------------------------
BaseOfStack		          equ 07c00h	    ; base addr of stack, grow from it
BaseOfLoader              equ 09000h        ; loader.bin is loaded here, seg addr
OffsetOfLoader            equ 0100h         ;                            offset addr
; ---------------------------------------------------------------
	
	jmp short LABEL_START          ; start to boot
	nop                            ; must add nop here!

%include        "fat12hdr.inc"

LABEL_START:
		mov	    ax, cs
		mov    	ds, ax
		mov 	es, ax
		mov     ss, ax
		mov     sp, BaseOfStack

		; clear the screen
		mov     ax, 0600h            ; AH = 6, AL = 0h
		mov     bx, 0700h            ; black back, white char
		mov     cx, 0                ; upper left corner (0, 0)
		mov     dx, 0184fh           ; lower right corber (80, 50)
		int     10h

		mov     dh, 0                ; "Booting  "
		call    DispStr

		; reset floppy disk driver
		xor     ah, ah
		xor     dl, dl
		int     13h

		; search LOADER.BIN in the root directory of A:      ; IMPORTANT: The assembler treats the backslash (\) followed by end-of-line sequence as white space
		mov     word [wSectorNo], SectorNoOfRootDirectory   ; start from 19
LABEL_SEARCH_IN_ROOT_DIR_BEGIN:
		cmp     word [wRootDirSizeForLoop], 0               ; loop through all 14 sectors for root dir
		jz      LABEL_NO_LOADERBIN
		dec     word [wRootDirSizeForLoop]
		mov     ax, BaseOfLoader
		mov     es, ax
		mov     bx, OffsetOfLoader
		mov     ax, [wSectorNo]                             ; read start from sector 19
		mov     cl, 1                                       ; read 1 sector
		call    ReadSector

		mov     si, LoaderFileName                          ; ds:si <- "LOADER  BIN"
		mov     di, OffsetOfLoader                          ; es:di <- BaseOfLoader:0100
		cld
		mov     dx, 10h
LABEL_SEARCH_FOR_LOADERBIN:
		cmp     dx, 0
		jz      LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR
		dec     dx
		mov     cx, 11                                      ; compare the loader filename, length is 11
LABEL_CMP_FILENAME:
		cmp     cx, 0
		jz      LABEL_FILENAME_FOUND
		dec     cx
		lodsb                                               ; al <- ds:si
		cmp     al, byte [es:di]
		jz      LABEL_GO_ON                                 ; if same, go on
		jmp     LABEL_DIFFERENT                             ; otherwise, go to next sector to search

LABEL_GO_ON:
		inc     di
		jmp     LABEL_CMP_FILENAME

LABEL_DIFFERENT:
		and     di, 0FFE0h                                  ; point to the start of the current dir
		add     di, 20h                                     ; move to next dir
		mov     si, LoaderFileName
		jmp     LABEL_SEARCH_FOR_LOADERBIN

LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR:
		add     word [wSectorNo], 1
		jmp     LABEL_SEARCH_IN_ROOT_DIR_BEGIN

LABEL_NO_LOADERBIN:
		mov     dh, 2                ; show the string of sequence #2
		call    DispStr

 		jmp     $

LABEL_FILENAME_FOUND:
		mov     ax, RootDirSectors
		and     di, 0FFE0h           ; start of current dir entry
		add     di, 01Ah             ; starting sector #, see Table 4.2
		mov     cx, word [es:di]
		push    cx                   ; save the # in FAT for current sector
		add     cx, ax
		add     cx, DeltaSectorNo
		mov     ax, BaseOfLoader
		mov     es, ax
		mov     bx, OffsetOfLoader
		mov     ax, cx
LABEL_GOON_LOADING_FILE:
		; add '.' each time reading a sector, Booting .....
		push    ax
		push    bx
		mov     ah, 0Eh
		mov     al, '.'
		mov     bl, 0Fh
		int     10h
		pop     bx
		pop     ax

		mov     cl, 1
		call    ReadSector
		pop     ax
		call    GetFATEntry
		cmp     ax, 0FFFh      ; if it is the last one, stop; otherwise continue to read
		jz      LABEL_FILE_LOADED
		push    ax
		mov     dx, RootDirSectors
		add     ax, dx              ; convert the sector # read from FAT to actual # in disk:
		add     ax, DeltaSectorNo   ; sector # + RootDirSecotrs (14) + (starting # of root dir (19) - 2) 
		add     bx, [BPB_BytsPerSec]; increase 512 to move to the next sector
		jmp     LABEL_GOON_LOADING_FILE
LABEL_FILE_LOADED:
		mov     dh, 1          ; print "Ready."
		call    DispStr

;---------- Jump to loader.bin and execute, handle the control to loader ---------------
		jmp     BaseOfLoader:OffsetOfLoader
;---------------------------------------------------------------------------------------

; --------------------------------------------------------
wRootDirSizeForLoop      dw  RootDirSectors      ; sector # taken by root directory, will decrement to 0
wSectorNo                dw  0                   ; the sector # to be read
bOdd                     db  0                   ; odd or even

LoaderFileName           db  "LOADER  BIN", 0    ; filename of LOADER.BIN
; to simplify, all string length is MessageLength
MessageLength            equ 9
BootMessage:             db  "Booting  "         ; sequence # 0
Message1                 db  "Ready.   "         ; sequence # 1
Message2                 db  "No LOADER"         ; sequence # 2
; --------------------------------------------------------

; show up the string, given the sequence # of string (0-based) in dh
DispStr:
	mov	    ax, MessageLength
	mul     dh
	add     ax, BootMessage      ; get the correct position of message from sequence #
	mov	    bp, ax
	mov     ax, ds               
	mov     es, ax               ; es:bp: string addr
	mov     cx, MessageLength    ; string length
	mov	    ax, 01301h
	mov	    bx, 0007h            ; BH = 0, black background, white char
	mov	    dl, 0
	int	    10h
	ret

; From ax-th sector, read number of sector (stored in cl) into es:bx
ReadSector: 
		; see Table4.4 for usage of "int 13h"
		; each floppy disk has:
		;	 2 heads (0, 1), each head has:
		;    80 tracks/columns (0-79), each track/column has:
		;    18 sectors (1-18)
		; the actural sector number is counted from 0
		; 1 track exists in 2 heads, e.g. track #12 exists on both head #0 and #1
		; actual sector num -> (column/track number, start sector num, head number)
		; Formula: Actual sector # = (head # + track/column # * 2) * 18 + (sector # - 1)
		push    bp
		mov     bp, sp
		sub     esp, 2     ; use stack to store the sector size (2 bytes)

		mov     byte [bp-2], cl
		push    bx    
		mov     bl, [BPB_SecPerTrk]        ; bl = 18, actual sector # / 18, quotient in al, remainder in ah
		div     bl                         ; see https://www.tutorialspoint.com/assembly_programming/assembly_arithmetic_instructions.htm
		inc     ah                         ; get the start sector #
		mov     cl, ah
		mov     dh, al          
		shr     al, 1                      ; get the track/column #
		mov     ch, al                  
		and     dh, 1                      ; get the head #
		pop     bx
		; now all three # are calculated
		mov     dl, [BS_DrvNum]
.GoOnReading:
		mov     ah, 2                      ; set to start read disk
		mov     al, byte [bp-2]            ; set the sector # to be read
		int     13h                        ; call the read disk interrupt
		jc      .GoOnReading               ; check CF, if failed (CF=1), continue to read

		add     esp, 2
		pop     bp

		ret

; find the FAT12 entry in sector numbered ax, and store the result in ax 
; the sector of FAT is stored in es:bx, or actually BaseOfLoader:OffsetOfLoader
;
; NOTICE: FAT is stored in Little Endian, need to revert the byte to have a correct read
; e.g. in the FAT example given in Page 107:
;                     0000200: F0 FF FF FF 8F 00 FF FF FF FF FF FF 09 A0 00 FF
; should be read as:           FF 00 A0 09 FF FF FF FF FF FF 00 8F FF FF FF F0
;                             high                                          low
; So for FAT12, read from low addr, the first 3 bytes are unused, sector 2 is FFF, 3 is 008, etc
; When given a sector #, say 5, we need to time the size of one FAT entry (1.5 or * 3 / 2)
; Then we get the actual offset in FAT is 7
; get the 2 bytes based on this offset, e.g. offset 7 -> FF FF
; and if the sector # is odd, right shift 4 bits, 
;                     is even, mask with 0x0FFF
; NOTICE: when we want to get the 2 bytes from the offset, remember the FAT can take up more than 1 sector
; in the disk, so we need to divide BPB_BytsPerSec and get the actual sector # and offset
GetFATEntry:
		push    es
		push    bx
		push    ax
		mov     ax, BaseOfLoader
		sub     ax, 0100h          ; leave 4k space for FAT
		mov     es, ax
		pop     ax
		mov     byte [bOdd], 0
		; get the offset in FAT for the sector number, * 3 / 2
		mov     bx, 3            
		mul     bx                 ; dx:ax = ax * 3
		mov     bx, 2
		div     bx                 ; dx:ax / 2 => quotient in ax, remainder in dx
		cmp     dx, 0              
		jz      LABEL_EVEN
		mov     byte [bOdd], 1
LABEL_EVEN: 
        ; now ax is the offset in FAT, continue to calculate the sector # in the disk based on this offset
		xor     dx, dx
		mov     bx, [BPB_BytsPerSec]
		div     bx    ;   dx:ax / BPB_BytsPerSec
		              ;   quotient in ax: sector # w.r.t FAT starting sector #; remainder in dx: offset in the sector
		push    dx
		mov     bx, 0
		add     ax, SectorNoOfFAT1    ; now ax is the sector # in the disk
		mov     cl, 2
		call    ReadSector       ; read the sector contains FATEntry, each time read 2 since one entry can be crossing 2 sectors
		pop     dx
		add     bx, dx
		mov     ax, [es:bx]
		cmp     byte [bOdd], 1
		jnz     LABEL_EVEN_2     
		shr     ax, 4            ; if odd, right shift by 4
LABEL_EVEN_2:
		and     ax, 0FFFh        ; if even, mask with lower 12 bits
LABEL_GET_FAT_ENTRY_OK:
		pop     bx
		pop     es
		ret

times	510-($-$$)	db	0
dw  0xaa55

