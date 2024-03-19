.section .data
A: .quad 10
B: .quad 5
.section .text
_soma:
    pushq %rbp
    movq %rsp, %rbp
    movq A, %rax
    addq B, %rax
    popq %rbp
    ret
_subtrai:
    pushq %rbp
    movq %rsp, %rbp
    movq A, %rax
    subq B, %rax
    popq %rbp
    ret
.global _start
_start:
    call _soma
    movq %rax, A
    call _subtrai
    movq %rax, B
    call _soma
    movq %rax, %rdi
    movq $60, %rax
    syscall

