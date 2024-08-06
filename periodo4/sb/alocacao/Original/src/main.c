#include <stdio.h>
#include "memalloc.h" 							

extern void *original_brk;	/* Você precisa ter a variável global que armazena o valor de brk como um extern aqui.
                            No código de teste estou chamandando de original_brk, mas se utilizarem outro nome,
                            substituir as ocorrências por ele aqui. */

int main() { 

    printf("============================== ROTINAS DE TESTE ==============================\n");

    setup_brk();
    void *initial_brk = original_brk;
    void *f_pnt, *s_pnt, *t_pnt, *m_pnt;


    if(!memory_free(original_brk))
        printf("SUCESSO\n");

    if(!memory_free(original_brk + 8))
        printf("SUCESSO\n");

    if(!memory_free(original_brk - 8))
        printf("SUCESSO\n");

    f_pnt = memory_alloc(100);
    printf("==>> ALOCANDO UM ESPAÇO DE 100 BYTES:\n");
    printf("\tLOCAL: %s\n", f_pnt-9 == initial_brk ? "CORRETO!" : "INCORRETO!");
    printf("\tIND. DE USO: %s\n", *((char*) (f_pnt-9)) == 1 ? "CORRETO!" : "INCORRETO!");
    printf("\tTAMANHO: %s\n", *((unsigned long int*) (f_pnt-8)) == 100 ? "CORRETO!" : "INCORRETO!");

    printf("==>> DESALOCANDO UM ESPAÇO DE 100 BYTES:\n");
    memory_free(f_pnt);
    printf("\tIND. DE USO: %s\n", *((char*) (f_pnt-9)) == 0 ? "CORRETO!" : "INCORRETO!");
    printf("\tTAMANHO: %s\n", *((unsigned long int*) (f_pnt-8)) == 100 ? "CORRETO!" : "INCORRETO!");

    s_pnt = memory_alloc(50);
    t_pnt = memory_alloc(100);
    printf("==>> ALOCANDO UM ESPAÇO DE 50 BYTES:\n");
    printf("\tLOCAL: %s\n", s_pnt-9 == initial_brk ? "CORRETO!" : "INCORRETO!");
    printf("\tIND. DE USO: %s\n", *((char*) (s_pnt-9)) == 1 ? "CORRETO!" : "INCORRETO!");
    printf("\tTAMANHO: %s\n", *((unsigned long int*) (s_pnt-8)) == 50 ? "CORRETO!" : "INCORRETO!");
    printf("==>> ALOCANDO UM ESPAÇO DE 100 BYTES:\n");
    printf("\tLOCAL: %s\n", s_pnt+100 == t_pnt-9 ? "CORRETO!" : "INCORRETO!");
    printf("\tIND. DE USO: %s\n", *((char*) (t_pnt-9)) == 1 ? "CORRETO!" : "INCORRETO!");
    printf("\tTAMANHO: %s\n", *((unsigned long int*) (t_pnt-8)) == 100 ? "CORRETO!" : "INCORRETO!");
    printf("==> VERIFICANDO A FRAGMENTAÇÃO DE MEMÓRIA:\n");
    printf("\tIND. DE USO: %s\n", *((char*) (s_pnt+50)) == 0 ? "CORRETO!" : "INCORRETO!");
    printf("\tTAMANHO: %s\n", *((unsigned long int*) (s_pnt+51)) == 41 ? "CORRETO!" : "INCORRETO!");

    printf("==>> DESALOCANDO:\n");
    memory_free(s_pnt);
    memory_free(t_pnt);
    printf("\tIND. DE USO: %s\n", *((char*) (s_pnt-9)) == 0 ? "CORRETO!" : "INCORRETO!");
    printf("\tTAMANHO: %s\n", *((unsigned long int*) (s_pnt-8)) == 50 ? "CORRETO!" : "INCORRETO!");
    printf("\tIND. DE USO: %s\n", *((char*) (t_pnt-9)) == 0 ? "CORRETO!" : "INCORRETO!");
    printf("\tTAMANHO: %s\n", *((unsigned long int*) (t_pnt-8)) == 100 ? "CORRETO!" : "INCORRETO!");

    printf("==>> VERIFICANDO A DESFRAGMENTAÇÃO DE MEMÓRIA:\n");
    m_pnt = memory_alloc(150);
    printf("==>> ALOCANDO UM ESPAÇO DE 150 BYTES:\n");
    printf("\tLOCAL: %s\n", m_pnt-9 == initial_brk ? "CORRETO!" : "INCORRETO!");
    printf("\tIND. DE USO: %s\n", *((char*) (m_pnt-9)) == 1 ? "CORRETO!" : "INCORRETO!");
    printf("\tTAMANHO: %s\n", *((unsigned long int*) (m_pnt-8)) == 150 ? "CORRETO!" : "INCORRETO!");

    printf("==>> DESALOCANDO TUDO:\n");
    memory_free(m_pnt);
    printf("\tIND. DE USO: %s\n", *((char*) (m_pnt-9)) == 0 ? "CORRETO!" : "INCORRETO!");
    printf("\tTAMANHO: %s\n", *((unsigned long int*) (m_pnt-8)) == 150 ? "CORRETO!" : "INCORRETO!");

    printf("==>> DESALOCANDO A PILHA (ILEGAL):\n");
    unsigned long long stack_var = 0;
    unsigned int alloc_return = memory_free((void*) &stack_var);
    if (!alloc_return) printf("\tO RETORNO DA LIBERAÇÃO FOI NULL!\n");

    return 0;
}
