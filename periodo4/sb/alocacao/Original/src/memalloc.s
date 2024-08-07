# =========================================================================
#  Authored by: Lucas Emanuel de Oliveira Santos & Thiago Fabricio de Mello
#  Contact: lucasde@ufpr.br & thiago.mello@ufpr.br
#  Date: 17/07/2024
# =========================================================================

.global setup_brk
.global dismiss_brk
.global search_block
.global memory_alloc
.global memory_free

.section .data
	.globl original_brk
	.globl current_brk
	original_brk: .quad 0
	current_brk: .quad 0
	STATE: .quad 1 # 1 byte indicando o estado do bloco
	SIZE: .quad 8 # 8 bytes indicando o tamanho do bloco
	HEADER: .quad 9 # tamanho do registro

.section .text

# Obtém o endereço de brk inicial
setup_brk:
	movq $12, %rax
	movq $0, %rdi
	syscall
	movq %rax, original_brk
	movq %rax, current_brk
	jmp _end_

# Restaura o endereço de brk para o estado inicial
dismiss_brk:
	movq $12, %rax
	movq original_brk, %rdi
	movq %rdi, current_brk
	syscall
	jmp _end_

# Procura o maior bloco disponível e funde blocos livres adjacentes
# %rdi = bytes
# -8(%rbp) = current_block
# -16(%rbp) = largest_block
# -24(%rbp) = current_size
# -32(%rbp) = largest_size
# -40(%rbp) = next_block
# -48(%rbp) = bytes
search_block:
        pushq %rbp
        movq %rsp, %rbp

        # Empilha as variáveis locais
        pushq original_brk
        pushq $0
        pushq $0
        pushq $0
        pushq $0
        pushq %rdi

        # If antes do while()
        movq original_brk, %rbx
        movq current_brk, %rcx
        cmp %rbx, %rcx
        jne _while_begin

        movq $12, %rax
        movq %rdi, %rbx
        movq current_brk, %rdi
        addq %rbx, %rdi
        addq HEADER, %rdi
        syscall
        movq %rax, current_brk
        movq -8(%rbp), %rax
        jmp _return_search

	_while_begin:
        movq -8(%rbp), %rbx
        movq current_brk, %rcx
        cmp %rcx, %rbx
        jge _while_end

        # Atualiza current_size
        movq -8(%rbp), %rax
        addq STATE, %rax
        movq (%rax), %rbx
        movq %rbx, -24(%rbp)

        # Atualiza next_block
        movq -8(%rbp), %rax
        movq -24(%rbp), %rbx
        addq %rbx, %rax
        addq HEADER, %rax
        movq %rax, -40(%rbp)

        # Primeira condição do 1º if()
        movq current_brk, %rax
        movq -40(%rbp), %rbx
        cmp %rax, %rbx
        je _else

        # Segunda condição do 1º if()
        movq -8(%rbp), %rax
        movb (%rax), %al
        movb $0, %ah
        cmp %ah, %al
        jne _else

        # Terceira condição do 1º if()
        movq -40(%rbp), %rax
        movb (%rax), %al
        movb $0, %ah
        cmp %ah, %al
        jne _else

        # Computa o 1º if()

        # Carrega o next_size
        movq -40(%rbp), %rax
        addq STATE, %rax
        movq (%rax), %rbx

        # Atualiza o current_size na pilha
        movq -24(%rbp), %rax
        addq %rbx, %rax
        addq HEADER, %rax
        movq %rax, -24(%rbp)

        # Atualiza o current_size na heap
        movq -8(%rbp), %rbx
        addq STATE, %rbx
        movq %rax, (%rbx)

        jmp _while_begin

	_else:
        # Primeira condição do 2º if()
        movq -8(%rbp), %rax
        movb (%rax), %al
        movb $0, %ah
        cmp %ah, %al
        jne _iteraction

        # Segunda condição do 2º if()
        movq -24(%rbp), %rax
        cmp %rdi, %rax
        jl _iteraction

        # Primeira condição do 3º if()
        movq -16(%rbp), %rax
        movq $0, %rbx
        cmp %rbx, %rax
        je _third_if

        # Segunda condição do 3º if()
        movq -32(%rbp), %rax
        movq -24(%rbp), %rbx
        cmp %rbx, %rax
        jl _third_if

        jmp _iteraction

	_third_if:
        movq -8(%rbp), %rax
        movq %rax, -16(%rbp)
        movq -24(%rbp), %rax
        movq %rax, -32(%rbp)

	_iteraction:
        movq -8(%rbp), %rax
        movq -24(%rbp), %rbx
        addq HEADER, %rax
        addq %rbx, %rax
        movq %rax, -8(%rbp)
        jmp _while_begin

	_while_end:
        # if() depois do while()
        movq -16(%rbp), %rax
        movq $0, %rbx
        cmp %rbx, %rax
        jne _return_largest

        # Computa o último if()
        movq current_brk, %rax
        movq %rax, -8(%rbp)
        movq $12, %rax
        movq %rdi, %rbx
        movq current_brk, %rdi
        addq %rbx, %rdi
        addq HEADER, %rdi
        syscall
        movq %rax, current_brk
        movq -8(%rbp), %rax
        jmp _return_search

	_return_largest:
        movq -16(%rbp), %rax

	_return_search:
	    addq $48, %rsp
        popq %rbp
        ret
        jmp _end_

# Aloca memória de tamanho bytes na heap
# -8(%rbp) = block
# -16(%rbp) = next_block
# -24(%rbp) = last_size
# -32(%rbp) = bytes
memory_alloc:
        pushq %rbp
        movq %rsp, %rbp
        pushq $0
        pushq $0
        pushq $0
        pushq %rdi

        call search_block
        movq %rax, -8(%rbp)
        movq -32(%rbp), %rdi

        # Primeiro if()
        movq current_brk, %rbx
        subq HEADER, %rbx
        subq %rdi, %rbx
        cmp %rbx, %rax
        jne _no_first_block

        movq -8(%rbp), %rax
        movb $1, (%rax)
        addq STATE, %rax
        movq %rdi, (%rax)
        jmp _return_malloc

    _no_first_block:
		movq -8(%rbp), %rax
		movb $1, (%rax)

		addq HEADER, %rax
		addq %rdi, %rax
		movq %rax, -16(%rbp)
		movq -8(%rbp), %rax
		addq STATE, %rax
		movq (%rax), %rbx
		movq %rbx, -24(%rbp)

		# Segundo if()
		movq current_brk, %rax
		movq -16(%rbp), %rbx
		cmp %rax, %rbx
		jge _return_malloc

		# Terceiro if()
		movq %rdi, %rax
		addq HEADER, %rax
		movq -24(%rbp), %rbx
		cmp %rax, %rbx
		jle _return_malloc

		movq -8(%rbp), %rax
		addq STATE, %rax
		movq %rdi, (%rax)

		movq -16(%rbp), %rax
		movb $0, (%rax)
		addq STATE, %rax
		movq -24(%rbp), %rbx
		subq %rdi, %rbx
		subq HEADER, %rbx
		movq %rbx, (%rax)

	_return_malloc:
        movq -8(%rbp), %rax
        addq HEADER, %rax

        addq $32, %rsp
        popq %rbp
        ret
        jmp _end_

memory_free:
		# Primeira condição do if()
		subq HEADER, %rdi
		movq current_brk, %rbx
		cmp %rbx, %rdi
		jge _free_yes_if

		# Segunda condição do if()
		movq original_brk, %rbx
		cmp %rbx, %rdi
		jl _free_yes_if

		# Terceira condição do if()
		movq $0, %rbx
		movq (%rdi), %rax
		cmp %rbx, %rax
		je _free_yes_if

		jmp _free_no_if

	_free_yes_if:
		movq $0, %rax
		jmp _return_free

	_free_no_if:
		movb $0, (%rdi)
		movq $1, %rax

	_return_free:

_end_:
