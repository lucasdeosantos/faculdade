#include <stdio.h>

typedef struct racional
{
    int num;
    int den;
}racionais;

racionais ler()
{
    racionais r;

    scanf("%d %d", &r.num,&r.den);

    return r;
}

int valida(racionais r)
{
    return r.den;
}

int mdc(racionais res)
{
    racionais r;
    int numCopia;

    if (res.den > res.num) 
    {
        r.num = res.den;
        r.den = res.num;
    }
    else
    {
        r.num = res.num;
        r.den = res.den;
    }

    while ((r.num%r.den) != 0) 
    {
        numCopia = r.num;
        r.num = r.den;
        r.den = numCopia%r.den;
    }

    return r.den;
}

void imprime(racionais r)
{
    int m;

    m = mdc(r);

    r.num = r.num/m;
    r.den = r.den/m;

        if(r.den==1)
            printf("%d\n",r.num);
        else
            if(r.den==-1)
                printf("%d\n",-r.num);
            else
                if(r.den<0)
                    printf("%d/%d\n",-r.num,-r.den);
                else
                    printf("%d/%d\n",r.num,r.den);
            
    return;
}

void soma(racionais r1, racionais r2)
{
    racionais res;

    res.num = r1.num * r2.den + r2.num * r1.den;
    res.den = r1.den * r2.den;
    
    if(res.num==0)
        printf("0\n");
    else
        imprime(res);
    
    return;
}

void sub(racionais r1, racionais r2)
{
    racionais res;

    res.num = r1.num * r2.den - r2.num * r1.den;
    res.den = r1.den * r2.den;

    if(res.num==0)
        printf("0\n");
    else
        imprime(res);
    
    return;
}

void prod(racionais r1, racionais r2)
{
    racionais res;

    res.num = r1.num * r2.num;
    res.den = r1.den * r2.den;

    if(res.num==0)
        printf("0\n");
    else
        imprime(res);

    return;
}

void divi(racionais r1,racionais r2)
{
    racionais res;

    res.num = r1.num * r2.den;
    res.den = r1.den * r2.num;

    if(res.num==0)
        printf("0\n");
    else
        if(!valida(res))
            printf("indeterminacao\n");
        else
            imprime(res);

    return;
}

int main()
{
    racionais r1,r2;

    r1 = ler(r1);
    if(valida(r1))
    {
        r2 = ler(r2);
        if(valida(r2))
        {
            soma(r1,r2);
            sub(r1,r2);
            prod(r1,r2);
            divi(r1,r2);
        }
        else
            printf("indeterminacao\n");
    }
    else
        printf("indeterminacao\n");
       
    return 0;
}
