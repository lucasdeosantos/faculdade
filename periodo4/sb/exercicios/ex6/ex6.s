.section .text
.global _start
_verifica_delta:
    pushq %rbp
    movq %rsp, %rbp
    # inicia as variaveis locais a, b e c
    pushq $1 # a = 1
    pushq $2 # b = 2
    pushq $-1 # c = -1
    # calcula delta = b*b - 4*a*c
    movq -16(%rbp), %rax
    imulq %rax, %rax
    movq $4, %rbx
    imulq -8(%rbp), %rbx
    imulq -24(%rbp), %rbx
    subq %rbx, %rax
    # se delta > 0 retorne 1, caso contrario retorne 0
    cmp $0, %rax
    jg _delta_positivo
    # delta negativo, retorna 0
    movq $0, %rax   
    jmp _fim_if
    _delta_positivo:
    # delta positivo, retorna 1
    movq $1, %rax
    _fim_if:
    # remove as variaveis locais da pilha
    addq $24, %rsp
    popq %rbp
    ret
_start:
    call _verifica_delta
    movq %rax, %rdi
    movq $60, %rax
    syscall
