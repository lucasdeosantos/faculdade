#include <stdio.h>
#include "lib_racionais.h"

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

racionais simplifica(racionais r)
{
    int m;
    
    if(r.num==0 || r.den==0)
    {
        r.num = 0;
        r.den = 0;
        return r;    
    }
    else
    {
        m = mdc(r);
        r.num = r.num/m;
        r.den = r.den/m;
           if(r.den < 0)
            {
                r.den = -1*r.den;
                r.num = -1*r.num;
            }
        return r;
    }

}

void imprime(racionais r)
{
        if(r.den==1)
            printf("%d\n",r.num);
        else
            if(r.den==0)
                printf("0\n");
            else
                printf("%d/%d\n",r.num,r.den);
            
    return;
}

racionais soma(racionais r1, racionais r2)
{
    racionais res;

    res.num = r1.num * r2.den + r2.num * r1.den;
    res.den = r1.den * r2.den;
    
    return simplifica(res);
}

racionais sub(racionais r1, racionais r2)
{
    racionais res;

    res.num = r1.num * r2.den - r2.num * r1.den;
    res.den = r1.den * r2.den;
    
    return simplifica(res);
}

racionais prod(racionais r1, racionais r2)
{
    racionais res;

    res.num = r1.num * r2.num;
    res.den = r1.den * r2.den;

    return simplifica(res);
}

racionais divi(racionais r1,racionais r2)
{
    racionais res;

    res.num = r1.num * r2.den;
    res.den = r1.den * r2.num;

    return simplifica(res);
}
