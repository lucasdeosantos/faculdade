.section .text
.global _start
_start:
    movq $0, %rdi
    movq $60, %rax
    syscall
