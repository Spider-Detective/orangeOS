; Prepare a _start function for ld linker as an entry point
extern main
extern exit

bits 32
[section .text]

global _start

_start:
        ; prepare argc and argv for main()
        push    eax
        push    ecx
        ; call main()
        call    main
        ; pass the return value of main() to exit()
        push    eax
        call    exit

        hlt         ; should not arrive here