; Store the base:offset addresses for loader and kernel
BaseOfLoader              equ 09000h             ; loader.bin is loaded here, seg addr
OffsetOfLoader            equ 0100h              ;                            offset addr

BaseOfLoaderPhyAddr       equ BaseOfLoader*10h   ; Physical addr of loader.bin loaded addr

BaseOfKernelFile          equ 08000h             ; kernel.bin is loaded here, seg addr
OffsetOfKernelFile        equ     0h             ;                            offset addr