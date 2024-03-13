#include <stdio.h>
#include "pilha.h"

void imprimeTabuleiro(int v[], int n){

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++)
            if(v[i] == j)
                printf("1 ");
            else
                printf("0 ");
        printf("\n");
    }
    printf("\n");

    return;
}

void rainhasRecursivo(int v[], int n, int r){

    if(r == n){
        imprimeTabuleiro(v, n);
        return;
    }
    for(int i = 0; i < n; i++){
        int bool = 1;
        for(int j = 0; j < r; j++){
            if(v[j] == i || v[j] == i+r-j || v[j] == i-r+j)
                bool = 0;
        }
        if(bool){
            v[r] = i;
            rainhasRecursivo(v, n, r+1);
        }
    }
    return;
}

void rainhas(int v[], int n, int r){
    t_pilha *p;

    p = cria_pilha(n + 1);

    empilha(r, p);

    while(!pilha_vazia(p)){

        desempilha(&r, p);

        if(r == n)
            imprimeTabuleiro(v, n);

        for(int i = 0; i < n; i++){
            int bool = 1;
            for(int j = 0; j < r; j++){
                if(v[j] == 1 || v[j] == i+r-j || v[j] == i-r+j)
                    bool = 0;
            }
            if(bool){
                v[r] = i;
                empilha(r+1, p);
                break;
            }
        }
    }
    destroi_pilha(p);
}

int main(){
    int n;
    scanf("%d", &n);

    int v[n];
    // rainhas(v, n, 0);
    rainhasRecursivo(v, n, 0);

    return 0;
}
