/* Cria com calloc() matriz 'n x m' */
double **criaMatriz (int n, int m);

/* Retorna ponteiro para matriz inversa de matriz 'A'.
   Matriz inversa é alocada com 'criaMatriz()'
*/
double **invMatriz (double **A, int n);

/* Libera matriz com 'n' linhas, alocada com 'criaMatriz()' */
void freeMatriz (double **A, int n);

/* Multiplica matrizes 'A' e 'B', ambas 'n x n' */
double **multMatriz (double **A, double **B, int n);

/* Mostra na tela matriz 'n x n' alocada com 'criaMatriz()' */
void prnMatriz (double **A, int n);

/* Preenche matriz 'n x n' alocada com 'criaMatriz()' */
void lerMatriz (double **A, int n);

