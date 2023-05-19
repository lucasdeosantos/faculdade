typedef struct racional
{
    int num;
    int den;

}racionais;

racionais ler();
/* le um numero racional */

int valida(racionais r);
/* valida se o denominador do numero irracional é diferente de zero */

void imprime(racionais r);
/* imprime um numero racional */

racionais soma(racionais r1, racionais r2);
/* calcula a soma de dois racionais */

racionais sub(racionais r1, racionais r2);
/* calcula a subtracao de dois racionais */

racionais prod(racionais r1, racionais r2);
/* calcula a multiplicacao de dois racionais */

racionais divi(racionais r1,racionais r2);
/* calcula a divisao de dois racionais  */
