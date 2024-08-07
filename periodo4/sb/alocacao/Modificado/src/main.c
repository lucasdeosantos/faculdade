#include <stdio.h>
#include <stdlib.h>

#include "memalloc.h" 							

extern void *original_brk;	/* Você precisa ter a variável global que armazena o valor de brk como um extern aqui.
							No código de teste estou chamandando de original_brk, mas se utilizarem outro nome,
							substituir as ocorrências por ele aqui. */

int main() { 

	printf("============================== ROTINAS DE TESTE ==============================\n");
	
	setup_brk();
	void *initial_brk = original_brk;
	void *f_pnt, *s_pnt, *t_pnt, *n4_pnt, *n5_pnt;

	f_pnt = memory_alloc(100);
	printf("==>> ALOCANDO UM ESPAÇO DE 100 BYTES:\n");
	printf("\tLOCAL: %s\n", f_pnt-9 == initial_brk ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((char*) (f_pnt-9)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (f_pnt-8)) == 100 ? "CORRETO!" : "INCORRETO!");

	printf("==>> DESALOCANDO UM ESPAÇO DE 100 BYTES:\n");
	memory_free(f_pnt);
	printf("\tIND. DE USO: %s\n", *((char*) (f_pnt-9)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (f_pnt-8)) == 100 ? "CORRETO!" : "INCORRETO!");

	s_pnt = memory_alloc(50);
	t_pnt = memory_alloc(100);
	printf("==>> ALOCANDO UM ESPAÇO DE 50 BYTES:\n");
	printf("\tLOCAL: %s\n", s_pnt-9 == initial_brk ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((char*) (s_pnt-9)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (s_pnt-8)) == 50 ? "CORRETO!" : "INCORRETO!");
	printf("==>> ALOCANDO UM ESPAÇO DE 100 BYTES:\n");
	printf("\tLOCAL: %s\n", s_pnt+100 == t_pnt-9 ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((char*) (t_pnt-9)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (t_pnt-8)) == 100 ? "CORRETO!" : "INCORRETO!");
	printf("==> VERIFICANDO A FRAGMENTAÇÃO DE MEMÓRIA:\n");
	printf("\tIND. DE USO: %s\n", *((char*) (s_pnt+50)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (s_pnt+51)) == 41 ? "CORRETO!" : "INCORRETO!");

    n4_pnt = memory_alloc(50);
    printf("==>> ALOCANDO UM ESPAÇO DE 50 BYTES:\n");
    printf("\tLOCAL: %s\n", n4_pnt-9 == t_pnt+100 ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((char*) (n4_pnt-9)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (n4_pnt-8)) == 50 ? "CORRETO!" : "INCORRETO!");
    
    printf("==>> DESALOCANDO UM ESPAÇO DE 50 BYTES:\n");
	memory_free(n4_pnt);
	printf("\tIND. DE USO: %s\n", *((char*) (n4_pnt-9)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (n4_pnt-8)) == 50 ? "CORRETO!" : "INCORRETO!");     
  
    n5_pnt = memory_alloc(41);
    printf("==>> ALOCANDO UM ESPAÇO DE 41 BYTES:\n");
    printf("\tLOCAL: %s\n", n5_pnt-9 == s_pnt+50 ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((char*) (n5_pnt-9)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (n5_pnt-8)) == 41 ? "CORRETO!" : "INCORRETO!");

	printf("==>> DESALOCANDO TUDO:\n");
    dismiss_brk();

	return 0;
}
