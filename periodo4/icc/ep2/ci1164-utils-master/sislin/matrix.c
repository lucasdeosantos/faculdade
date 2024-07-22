#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

/* Cria com calloc() matriz 'n x m' */
double **criaMatriz (int n, int m)
{
  double **C;
  int i, j;
  
  C = (double **) calloc(n,sizeof(double *));
  C[0] = (double *) calloc(n*m,sizeof(double));
  for (i=1; i < n; ++i)
    C[i] = C[i-1] + m;

  return C;
}

/* Retorna ponteiro para matriz inversa de matriz 'A'.
   Matriz inversa é alocada com 'criaMatriz()'
*/
// Adaptação de https://www.programming-techniques.com/2011/09/numerical-methods-inverse-of-nxn-matrix.html
double **invMatriz (double **A, int n)
{
  double ratio,a, **aux, **IA;
  int i, j, k;

  aux = criaMatriz(n, 2*n);
  IA = criaMatriz(n,n);
  
  for(i = 0; i < n; i++) {
    for(j = 0; j < 2*n; j++) {
      if (j < n)
	aux[i][j] = A[i][j];
      else if(i==(j-n))
	aux[i][j] = 1.0;
      else
	aux[i][j] = 0.0;
    }
  }

  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      if(i!=j){
	ratio = aux[j][i]/aux[i][i];
	for(k = 0; k < 2*n; k++){
	  aux[j][k] -= ratio * aux[i][k];
	}
      }
    }
  }

  for(i = 0; i < n; i++){
    a = aux[i][i];
    for(j = n; j < 2*n; j++){
      IA[i][j-n] = aux[i][j] / a;
    }
  }

  freeMatriz(aux, n);
  
  return IA;

}

/* Libera matriz com 'n' linhas, alocada com 'criaMatriz()' */
void freeMatriz (double **A, int n)
{
  int i;
  
  free(A[0]);
  free(A);
}

/* Multiplica matrizes 'A' e 'B', ambas 'n x n' */
double **multMatriz (double **A, double **B, int n)
{
  double **C;
  int i, j,k;

  C = (double **) malloc(n*sizeof(double *));
  for (i=0; i < n; ++i) {
    C[i] = (double *) calloc(n,sizeof(double));
    for (j=0; j < n; ++j)
      for (k=0; k < n; ++k)
	C[i][j] += A[i][k] * B[k][j];
  }

  return C;
}

/* Mostra na tela matriz 'n x n' alocada com 'criaMatriz()' */
void prnMatriz (double **A, int n)
{
  int i, j;
  
  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      printf("%15.5lg", A[i][j]);
    }
    printf("\n");
  }
}

/* Preenche matriz 'n x n' alocada com 'criaMatriz()' */
void lerMatriz (double **A, int n)
{
  int i, j;
  
  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      scanf("%lg", A[i]+j);
    }
  }
}

