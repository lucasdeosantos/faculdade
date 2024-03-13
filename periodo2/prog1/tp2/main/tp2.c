#include <stdio.h>
#include "lib_racionais.h"

int main()
{
    racionais r1,r2,s,b,p,d;

    while(1)
    {

        r1 = ler(r1);
        if(valida(r1))
        {
            r2 = ler(r2);
            if(valida(r2))
            {
                s = soma(r1,r2);
                imprime(s);
                b = sub(r1,r2);
                imprime(b);
                p = prod(r1,r2);
                imprime(p);
                d = divi(r1,r2);
                imprime(d);
            }
            else
                break;

        }
        else
            break;     
    }

    return 0;
}
