; Library functions written in assembly
;

%include "sconst.inc"

extern disp_pos

[SECTION .text]

global  disp_str
global  disp_color_str
global  out_byte
global  in_byte
global  enable_irq
global  disable_irq
global  disable_int
global  enable_int

;; show up a string, void disp_str(char * info);
disp_str:
        push    ebp
        mov     ebp, esp

        mov     esi, [ebp + 8]    ; pszInfo, see start.c
        mov     edi, [disp_pos]
        mov     ah, 0Fh
.1:
        lodsb
        test    al, al            ; if encounter \0, stop
        jz      .2                
        cmp     al, 0Ah           ; check if the current char is \n
        jnz     .3                ; jump if (al != 0Ah), and show the char
        push    eax               ; if it is \n, update edi (current display position) to next line
        push    ebx               ; NOTICE: in protect mode, need to protect 
                                  ; ebx when using bl below, still don't know why
        mov     eax, edi
        mov     bl, 160           ; 160 is the length of the line
        div     bl                ; eax / 160, save result in eax
        and     eax, 0FFh
        inc     eax
        mov     bl, 160
        mul     bl
        mov     edi, eax
        pop     ebx 
        pop     eax
        jmp     .1
.3:
        mov     [gs:edi], ax
        add     edi, 2
        jmp     .1
.2: 
        mov     [disp_pos], edi

        pop     ebp
        ret
; end of disp_str

;; void disp_color_str(char * info, int color);
disp_color_str:
        push    ebp
        mov     ebp, esp

        mov     esi, [ebp + 8]    ; pszInfo, see start.c
        mov     edi, [disp_pos]
        mov     ah, [ebp + 12]
.1:
        lodsb
        test    al, al            ; if encounter \0, stop
        jz      .2                
        cmp     al, 0Ah           ; check if the current char is \n
        jnz     .3                ; jump if (al != 0Ah), and show the char
        push    eax               ; if it is \n, update edi (current display position) to next line
        push    ebx               ; NOTICE: in protect mode, need to protect 
                                  ; ebx when using bl below, still don't know why
        mov     eax, edi
        mov     bl, 160           ; 160 is the length of the line
        div     bl                ; eax / 160, save result in eax
        and     eax, 0FFh
        inc     eax
        mov     bl, 160
        mul     bl
        mov     edi, eax
        pop     ebx 
        pop     eax
        jmp     .1
.3:
        mov     [gs:edi], ax
        add     edi, 2
        jmp     .1
.2: 
        mov     [disp_pos], edi

        pop     ebp
        ret
; end of DispStr

; void out_byte(u16 port, u8 value);
out_byte:
        mov     edx, [esp + 4]     ; port
        mov     al, [esp + 4 + 4]  ; value
        out     dx, al
        nop
        nop
        ret

; void in_byte(u16 port)
in_byte:
        mov     edx, [esp + 4]      ; port
        xor     eax, eax
        in      al, dx
        nop
        nop
        ret

; disable the interrupt of 8259 for a given irq
; if (irq < 8)
;     out_byte(INT_M_CTLMASK, in_byte(INT_M_CTLMASK) | (1 << (irq % 8)));
; else
;     out_byte(INT_S_CTLMASK, in_byte(INT_S_CTLMASK) | (1 << (irq % 8)));
; if successfully disabled, set eax to be 1; otherwise (say already disabled) set to be 0
disable_irq:
        mov     ecx, [esp + 4]      ; irq number
        pushf
        cli
        mov     ah, 1
        rol     ah, cl              ; rotate cl bits to left, ah << (cl % 8)
        cmp     cl, 8
        jae     disable_8           ; go for slave
disable_0:
        in      al, INT_M_CTLMASK
        test    al, ah
        jnz     dis_already         ; already disabled
        or      al, ah
        out     INT_M_CTLMASK, al
        popf
        mov     eax, 1
        ret
disable_8:
        in      al, INT_S_CTLMASK
        test    al, ah
        jnz     dis_already
        or      al, ah
        out     INT_S_CTLMASK, al
        popf
        mov     eax, 1
        ret
dis_already:
        popf
        xor     eax, eax
        ret

; enable the interrupt of 8259 for a given irq
; if (irq < 8)
;     out_byte(INT_M_CTLMASK, in_byte(INT_M_CTLMASK) & ~(1 << (irq % 8)));
; else
;     out_byte(INT_S_CTLMASK, in_byte(INT_S_CTLMASK) & ~(1 << (irq % 8)));
enable_irq:
        mov     ecx, [esp + 4]      ; irq number
        pushf                       ; push all flags into stack
        cli
        mov     ah, ~1
        rol     ah, cl              ; ah = ~(1 << (cl % 8))
        cmp     cl, 8
        jae     enable_8
enable_0:
        in      al, INT_M_CTLMASK
        and     al, ah
        out     INT_M_CTLMASK, al   ; clear the given irq bit on 8259
        popf
        ret
enable_8:
        in      al, INT_S_CTLMASK
        and     al, ah
        out     INT_S_CTLMASK, al   ; clear the given irq bit on 8259
        popf
        ret

; void disable_int();
disable_int:
        cli
        ret

; void enable_int();
enable_int:
        sti
        ret