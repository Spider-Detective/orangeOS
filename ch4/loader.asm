; This is the loader that will be loaded by boot.asm when system boots.
; boot.asm will transfer the control to loader.asm, and loader.asm will continue to boot the system
org     0100h
        
        mov     ax, 0B800h
        mov     gs, ax
        mov     ah, 0Fh
        mov     al, 'L'
        mov     [gs:((80 * 0 + 39) * 2)], ax

        jmp     $            ; stop and enter infinite loop