.section .data
A: .quad 5
B: .quad 10
.section .text
.global _start
_start:
    movq A, %rdi
    addq B, %rdi
    movq $60, %rax
    syscall
