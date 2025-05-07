.global _start

_start:
    ldr sp, =stack_top
    bl kernel_main

    b .

.section .bss
    .space 4096
stack_top:
