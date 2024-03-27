.section .text
.global _start
_area:
    pushq %rbp
    movq %rsp, %rbp
    movq +16(%rbp), %rax
    imulq %rax, %rax # r*r
    imulq $3, %rax # pi = 3
    popq %rbp
    ret
_start:
    pushq %rbp
    movq %rsp, %rbp
    pushq $3
    pushq -8(%rbp)
    call _area
    addq $16, %rsp
    movq %rax, %rdi
    movq $60, %rax
    syscall
    
