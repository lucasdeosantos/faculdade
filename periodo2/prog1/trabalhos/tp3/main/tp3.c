#include <stdio.h>
#include <stdlib.h>
#include "lib_racionais.h"
#define MIN 0
#define MAX 100

int ler_tamanho ()
{
    int tam;

    while(1)
    {
        scanf("%d",&tam);    
        if(tam >= MIN && tam <= MAX-1)
            return tam;
    }
}

void imprimir_vetor_racional(racional **w, int tam)
{
    int i;

    for(i = MIN; i < tam; i++)
        imprimir_r(w[i]);
    printf("\n");

    return;
}


void liberar_vetor(racional **w, int tam)
{
    int i;

    for(i = MIN; i < tam; i++)
        liberar_r(w[i]);

    free(w);

    return;
}

racional** aleatorio_vetor_racional(int tam)
{ 
    int i;
    racional **v;

    v = malloc(sizeof(racional*)*tam);

    for(i = MIN; i < tam; i++)
        if((v[i] = sortear_r()) == NULL)
            return NULL;

    return v;
}

racional** ordenar_vetor_racional(racional **v, int tam)
{  
  int i, j, menor;
  racional *troca;
  
  for (i = MIN; i < (tam-1); i++)
   { 
    menor = i; 
    for (j = (i+1); j < tam; j++) 
      if(menor_r(v[j], v[menor]))
        menor = j; 
        
    if (i != menor) 
    { 
      troca = v[i]; 
      v[i] = v[menor]; 
      v[menor] = troca; 
    } 
  }
  return v;
}

int main()
{
    racional **v, **w; 
    int tam;

    srand(0);

    tam = ler_tamanho();
    v = aleatorio_vetor_racional (tam);
    w = ordenar_vetor_racional (v, tam); 
    imprimir_vetor_racional (w, tam);
    liberar_vetor(v,tam);

    return 0;
}