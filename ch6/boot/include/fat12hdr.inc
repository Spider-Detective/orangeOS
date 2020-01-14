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

; constants definition based on current FAT12 header
FATSz                     equ 9             ; same as BPB_FATSz16 
RootDirSectors            equ 14            ; space taken by root dir
SectorNoOfRootDirectory   equ 19            ; First sector # of root directory, see Figure 4.1
SectorNoOfFAT1            equ 1             ; Starting sector # of FAT1, relates to BPB_RsvdSecCnt
DeltaSectorNo             equ 17            ; = BPB_RsvdSecCnt + (BPB_NumFATs * BPB_FATSz16) - 2