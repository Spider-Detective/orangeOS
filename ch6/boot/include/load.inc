; Store the base:offset addresses for loader and kernel
BaseOfLoader              equ 09000h                  ; loader.bin is loaded here, seg addr
OffsetOfLoader            equ 0100h                   ;                            offset addr

BaseOfLoaderPhyAddr       equ BaseOfLoader*10h        ; Physical addr of loader.bin loaded addr

BaseOfKernelFile          equ 08000h                  ; kernel.bin is loaded here, seg addr
OffsetOfKernelFile        equ     0h                  ;                            offset addr

BaseOfKernelFilePhyAddr   equ BaseOfKernelFile*10h
KernelEntryPointPhyAddr   equ 030400h                 ; same as -Ttext in Makefile

PageDirBase               equ 100000h                 ; starting addr of page dir
PageTblBase               equ 101000h                 ; starting addr of page tbl