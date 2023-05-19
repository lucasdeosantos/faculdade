#include <stdio.h>
#include <stdlib.h>
#include "lib_racionais.h"
#define MIN 0
#define MAX 100

int aleat (int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

racional *criar_r ()
{
    racional *r;

    if(!(r = malloc(sizeof(racional))))
        return NULL;

    return r;
}

racional *liberar_r (racional *r)
{
    free(r);

    return r = NULL;
}

racional *sortear_r ()
{
    racional *r_sorteado = criar_r();

    r_sorteado->num = aleat(MIN,MAX);
    r_sorteado->den = aleat(MIN+1,MAX);

    simplifica_r(r_sorteado);

    return r_sorteado;
}

int ler_r (racional *r)
{
    if(scanf("%d %d", &r->num, &r->den) != 2)
        return 0;

    if(!r->den)
        return 0;

    return 1;
}

void imprimir_r (racional *r)
{
    if(r->den == 1)
        printf("%d ", r->num);
    else
        printf("%d/%d ", r->num, r->den);
}

int mdc (int a, int b)
{
    if(b == 0)
        return a;

    return mdc(b,a % b);
}

int mmc (int a, int b)
{
    return (a*b)/mdc(a,b);
}

int valido_r (racional *r)
{
    if(r->den != 0)
            return 1;
    
    return 0;
}

void simplifica_r (racional *r)
{
    int m;
    
    m = mdc(r->num, r->den);
    r->num = r->num/m;
    r->den = r->den/m;

    return;
}

int menor_r (racional *r1, racional *r2)
{
    if(r1->num * r2->den < r1->den * r2->num)
        return 1;

    return 0;
}

int iguais_r (racional *r1, racional *r2)
{
    simplifica_r(r1);
    simplifica_r(r2);

    if(r1->num == r2->num && r1->den == r2->den)
        return 1;

    return 0;
}

racional *somar_r (racional *r1, racional *r2)
{
    racional *pRes = criar_r();

    if(pRes == NULL)
        return NULL;

    pRes->num = r1->num * r2->den + r2->num * r1->den;
    pRes->den = r1->den * r2->den;
    
    simplifica_r(pRes);


    return pRes;
}

racional *subtrair_r (racional *r1, racional *r2)
{
    racional *pRes = criar_r();

    if(pRes == NULL)
        return NULL;

    pRes->num = r1->num * r2->den - r2->num * r1->den;
    pRes->den = r1->den * r2->den;
    
    simplifica_r(pRes);

    return pRes;
}

racional *multiplicar_r (racional *r1, racional *r2)
{
    racional *pRes = criar_r();

    if(pRes == NULL)
        return NULL;

    pRes->num = r1->num * r2->num;
    pRes->den = r1->den * r2->den;

    simplifica_r(pRes);

    return pRes;
}

racional *dividir_r (racional *r1, racional *r2)
{
    racional *pRes = criar_r();

    if(pRes == NULL)
        return NULL;

    pRes->num = r1->num * r2->den;
    pRes->den = r1->den * r2->num;

    simplifica_r(pRes);

    return pRes;
}