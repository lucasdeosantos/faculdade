.section .data
v: .quad 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
.section .text
.global _start
_start:
    movq $0, %rax
    movq $0, %rdi 
_loop_start:
    movq $1, %rbx
    andq v(, %rax, 8), %rbx
    cmpq $0, %rbx
    jne _impar
    addq $1, %rdi
_impar:
    addq $1, %rax
    cmpq $10, %rax
    je _loop_end
    jmp _loop_start
_loop_end:
    movq $60, %rax
    syscall
    
