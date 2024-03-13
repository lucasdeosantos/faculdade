struct s_pilha {
    int tam;
    int topo;
    int *v;
};
typedef struct s_pilha t_pilha;

/* Cria uma pilha com espaço para n elementos do tipo int. 
   Retorna um ponteiro para a pilha ou NULL em caso de erro na 
   alocacao de memoria. */
t_pilha* cria_pilha(int n);

/* Desaloca a memoria usada pela pilha e retorna NULL */
t_pilha* destroi_pilha(t_pilha *p);

/* Retorna 1 se a pilha p esta vazia e 0 caso contrario. */
int pilha_vazia(t_pilha *p);

/* Empilha o double x na pilha p. Retorna 1 se a operacao foi 
   realizada com sucesso e 0 caso contrario. */
int empilha (int x, t_pilha *p);

/* Retorna em t o elemento do topo da pilha e o desempilha. A funcao 
   retorna 1 se a operacao foi bem sucedida e 0 caso contrario. */
int desempilha(int *t, t_pilha *p);

/* Retorna em t o elemento do topo da pilha, sem desempilhar. 
   A funcao retorna 1 se a operacao foi bem sucedida e 0 caso 
   contrario.   */
int topo(int *t, t_pilha *p);
