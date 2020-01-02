
;%define	_BOOT_DEBUG_	; 做 Boot Sector 时一定将此行注释掉!将此行打开后可用 nasm Boot.asm -o Boot.com 做成一个.COM文件易于调试

%ifdef	_BOOT_DEBUG_
	org  0100h			; 调试状态, 做成 .COM 文件, 可调试
%else
	org  07c00h			; Boot 状态, Bios 将把 Boot Sector 加载到 0:7C00 处并开始执行
%endif

; ----------------- constants definitions ------------------------
%ifdef	_BOOT_DEBUG_
BaseOfStack		equ	0100h	; 调试状态下堆栈基地址(栈底, 从这个位置向低地址生长)
%else
BaseOfStack		equ	07c00h	; 堆栈基地址(栈底, 从这个位置向低地址生长)
%endif

BaseOfLoader              equ 09000h        ; loader.bin is loaded here, seg addr
OffsetOfLoader            equ 0100h         ;                            offset addr
RootDirSectors            equ 14            ; space taken by root dir
SectorNoOfRootDirectory   equ 19            ; First sector # of root directory, see Figure 4.1
; ---------------------------------------------------------------
	
	jmp short LABEL_START          ; start to boot
	nop                            ; must add nop here!

	; write the header of FAT12
	BS_OEMName        DB 'ForrestY'        ; OEM String, must be 8 bytes
	BPB_BytsPerSec    DW 512               ; Bytes per sector
	BPB_SecPerClus    DB 1                 ; sectors per cluster
	BPB_RsvdSecCnt    DW 1                 ; sector taken by boot record
	BPB_NumFATs       DB 2                 ; FAT table in total
	BPB_RootEntCnt    DW 224               ; max value of root dir file (32 bytes each)
	BPB_TotSec16      DW 2880              ; Num of logical sectors
	BPB_Media         DB 0xF0              ; media descriptor
	BPB_FATSz16       DW 9                 ; sectors per FAT
	BPB_SecPerTrk     DW 18                ; sector per track
	BPB_NumHeads      DW 2                 ; num of heads
	BPB_HiddSec       DD 0                 ; num of hidden sec
	BPB_TotSec32      DD 0                 ; 
	BS_DrvNum         DB 0                 ; driver number for interrupt 12
	BS_Reserved1      DB 0                 ; unused
	BS_BootSig        DB 29h               ; extended mark
	BS_VolID          DD 0                 ; volume id
	BS_VolLab         DB 'OrangeS0.02'     ; volume label, must be 11 byte
	BS_FileSysType    DB 'FAT12   '        ; filesystem type, must be 8 bytes

LABEL_START:
		mov	    ax, cs
		mov    	ds, ax
		mov 	es, ax
		mov     ss, ax
		mov     sp, BaseOfStack

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

;		jmp     $
%ifdef	_BOOT_DEBUG_
	mov	ax, 4c00h		; `.
	int	21h			; /  没有找到 LOADER.BIN, 回到 DOS
%else
	jmp	$			; 没有找到 LOADER.BIN, 死循环在这里
%endif

LABEL_FILENAME_FOUND:
		jmp     $                    ; stop here for now

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

times	510-($-$$)	db	0
dw  0xaa55

