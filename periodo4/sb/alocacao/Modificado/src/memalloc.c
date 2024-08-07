/* =========================================================================
    Authored by: Lucas Emanuel de Oliveira Santos & Thiago Fabricio de Mello
    Contact: lucasde@ufpr.br & thiago.mello@ufpr.br
    Date: 11/07/2024
   ========================================================================= */

#include <unistd.h> 
#include "memalloc.h"

#define STATE 1 // 1 byte indicando o estado do bloco
#define SIZE 8 // 8 bytes indicando o tamanho do bloco
#define HEADER 9 // tamanho do registro

void *original_brk, *current_brk;

// Obtém o endereço de brk inicial
void setup_brk() {
    original_brk = sbrk(0);
    current_brk = original_brk;
}

// Restaura o endereço de brk para o estado inicial
void dismiss_brk() {
    brk(original_brk);
    current_brk = original_brk;
}

// Procura o maior bloco disponível e funde blocos livres adjacentes
void *search_block(unsigned long int bytes) {
    void *current_block = original_brk;
    void *largest_block = NULL;
    unsigned long int current_size;
    unsigned long int largest_size = 0;

    if (current_brk == original_brk){
        sbrk(HEADER + bytes);
        current_brk = sbrk(0);
        return current_block;
    }

    while (current_block < current_brk) {
        current_size = *(unsigned long int *)(current_block + STATE);
        void *next_block = current_block + HEADER + current_size;

        if (next_block != current_brk && *(char *)current_block == 0 && *(char *)next_block == 0){
            unsigned long int next_size = *(unsigned long int *)(next_block + STATE);
            current_size = current_size + HEADER + next_size;
            *(unsigned long int *)(current_block + STATE) = current_size;
        }
        else {
            if (*(char *)current_block == 0 && current_size >= bytes) {
                if (!largest_block || largest_size > current_size) {
                    largest_block = current_block;
                    largest_size = current_size;
                }
            }

            current_block = current_block + HEADER + current_size;
        }
    }

    if (!largest_block){
        current_block = current_brk;
        sbrk(HEADER + bytes);
        current_brk = sbrk(0);
        return current_block;
    }

    return largest_block;
}

// Aloca memória de tamanho bytes na heap
void *memory_alloc(unsigned long int bytes) {
    void *block = search_block(bytes);

    if (block == current_brk - (HEADER + bytes)) {
        *(char *)(block) = 1;
        *(unsigned long int *)(block + STATE) = bytes;
        return block + HEADER;
    }

    *(char *)(block) = 1;

    void *next_block = block + HEADER + bytes;
    unsigned long int last_size = *((unsigned long int *)(block + STATE));

    if (next_block < current_brk){
        if (last_size > bytes + HEADER) {
            *(unsigned long int *)(block + STATE) = bytes;
            *(char *)next_block = 0;
            *(unsigned long int *)(next_block + STATE) = last_size - (bytes + HEADER);
        }
    }

    return block + HEADER;
}

// Libera memória apontada por pointer
int memory_free(void *pointer) {
    if (pointer - HEADER >= current_brk || pointer - HEADER < original_brk || *(char *)(pointer - HEADER) == 0)
        return 0;

    *(char *)(pointer - HEADER) = 0;

    return 1;
}
