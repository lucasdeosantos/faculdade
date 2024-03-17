.section .data
X: .quad 11
Y: .quad 33
.section .text
.global _start
_start:
    movq X, %rax
    cmpq Y, %rax
    movq $0, %rdi
    jge _end
    movq %rax, %rdi
_loop_start:
    cmpq Y, %rax
    jg _loop_end
    addq $1, %rax
    addq %rax, %rdi
    jmp _loop_start
_loop_end:
    cmpq $255, %rdi
    jl _end
    movq $255, %rdi
_end:
    movq $60, %rax
    syscall
