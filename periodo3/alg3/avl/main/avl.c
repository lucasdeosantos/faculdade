#include "avl.h"
#include "fila.h"

struct aluno *getAluno1()
{
	struct aluno *retorno = malloc(sizeof(struct aluno));
	if (!retorno)
		exit(1);
	retorno->nome = malloc(sizeof("Lucas Emanuel de Oliveira Santos"));
	if (!retorno->nome)
		exit(1);
	retorno->nomeDinf = malloc(sizeof("leos22"));
	if (!retorno->nomeDinf)
		exit(1);

	strcpy(retorno->nome, "Lucas Emanuel de Oliveira Santos");
	strcpy(retorno->nomeDinf, "leos22");
	retorno->grr = 20224379;

	return retorno;
}

struct aluno *getAluno2()
{
	struct aluno *retorno = malloc(sizeof(struct aluno));
	if (!retorno)
		exit(1);
	retorno->nome = malloc(sizeof("Frank Wolff Hannemann"));
	if (!retorno->nome)
		exit(1);
	retorno->nomeDinf = malloc(sizeof("fwh22"));
	if (!retorno->nomeDinf)
		exit(1);

	strcpy(retorno->nome, "Frank Wolff Hannemann");
	strcpy(retorno->nomeDinf, "fwh22");
	retorno->grr = 20224758;

	return retorno;
}

void imprimirDadosAlunos()
{
	struct aluno *aluno = getAluno1();

	printf("Trabalho de %s\n", aluno->nome);
	printf("Login Dinf %s\n", aluno->nomeDinf);
	printf("GRR %u\n\n", aluno->grr);

	free(aluno->nome);
	free(aluno->nomeDinf);
	free(aluno);

	aluno = getAluno2();
	if (!aluno)
		return;

	printf("...E... \n\n");
	printf("Trabalho de %s\n", aluno->nome);
	printf("Login Dinf %s\n", aluno->nomeDinf);
	printf("GRR %u\n\n", aluno->grr);

	free(aluno->nome);
	free(aluno->nomeDinf);
	free(aluno);
}

// aloca memoria para um nodo.
// retorna NULL em caso de erro na alocacao.
struct nodo *criaNodo(int chave)
{
	struct nodo *nodo = malloc(sizeof(struct nodo));

	if (!nodo)
		return NULL;

	nodo->chave = chave;
	nodo->fe = NULL;
	nodo->fd = NULL;
	nodo->pai = NULL;
	nodo->h = 0;

	return nodo;
}

// retorna o maior valor entre dois numeros.
int max(int a, int b)
{
	return a > b ? a : b;
}

// retorna a altura de um nodo, se o nodo
// for nulo retorna -1.
int altura(struct nodo *nodo)
{
	if (!nodo)
		return -1;

	return nodo->h;
}

// retorna o balanco de um nodo.
int balanco(struct nodo *nodo)
{
	return altura(nodo->fd) - altura(nodo->fe);
}

// atualiza a altura de um nodo.
// altura = max(alturaFilhoDireito, alturaFilhoEsquerdo) + 1.
void atualizaAltura(struct nodo *nodo)
{
	if (!nodo->fe && !nodo->fd)
		nodo->h = 0;
	else if (!nodo->fd)
		nodo->h = nodo->fe->h + 1;
	else if (!nodo->fe)
		nodo->h = nodo->fd->h + 1;
	else
		nodo->h = max(nodo->fd->h, nodo->fe->h) + 1;
}

void rotacaoEsquerda(struct nodo **raiz, struct nodo *x)
{
	struct nodo *y = x->fd;

	x->fd = y->fe;
	if (y->fe)
		y->fe->pai = x;
	y->pai = x->pai;
	if (x == *raiz)
		*raiz = y;
	else if (x == x->pai->fe)
		x->pai->fe = y;
	else
		x->pai->fd = y;

	y->fe = x;
	x->pai = y;
	atualizaAltura(x);
	atualizaAltura(y);
}

void rotacaoDireita(struct nodo **raiz, struct nodo *x)
{
	struct nodo *y = x->fe;

	x->fe = y->fd;
	if (y->fd)
		y->fd->pai = x;
	y->pai = x->pai;
	if (x == *raiz)
		*raiz = y;
	else if (x == x->pai->fd)
		x->pai->fd = y;
	else
		x->pai->fe = y;

	y->fd = x;
	x->pai = y;
	atualizaAltura(x);
	atualizaAltura(y);
}

void balancear(struct nodo **raiz, struct nodo *nodo)
{
	int b = balanco(nodo);

	// arvore pendendo pra direita.
	if (b == 2) {
		// se a sub-arvore direita pender pra esquerda
		// faz uma rotacao dupla esquerda.
		if (balanco(nodo->fd) == -1)
			rotacaoDireita(raiz, nodo->fd); 
		// senao, faz uma rotacao simples para a esquerda.
		rotacaoEsquerda(raiz, nodo);
	}
	// arvore pendendo pra esquerda.
	else if (b == -2) {
		// se a sub-arvore esquerda pender pra direita
		// faz uma rotacao dupla direita.
		if (balanco(nodo->fe) == 1)
			rotacaoEsquerda(raiz, nodo->fe);
		// senao, faz uma rotacao simples direita.
		rotacaoDireita(raiz, nodo);
	}
}

// retorna apos balancear a raiz.
void corrigirBalanco(struct nodo **raiz, struct nodo *nodo)
{
	if (!nodo)
		return;

	struct nodo *pai = nodo->pai; // garante que vai corrigir o pai do nodo atual.

	atualizaAltura(nodo); // atualiza a altura do nodo.
	balancear(raiz, nodo); // balanceia o nodo se necessario.
	return corrigirBalanco(raiz, pai); // corrige o balanco do pai.
}

struct nodo *inserir(struct nodo **raiz, int chave)
{
	struct nodo *nodo = criaNodo(chave);
	struct nodo *aux = *raiz;
	struct nodo *pai = NULL;

	if (!nodo)
		return NULL;

	// busca a posicao do nodo na arvore. 
	while (aux) {
		pai = aux;
		if (nodo->chave < aux->chave) {
			aux = aux->fe;
		}
		else if (nodo->chave > aux->chave) {
			aux = aux->fd;
		}
		else { // encontrou a chave na arvore.
			free(nodo);
			return NULL;
		}
	}
	nodo->pai = pai;
	if (!pai) { // arvore esta vazia.
		*raiz = nodo;
		return nodo;
	}
	else { // define se e filho esquerdo ou direito.
		if (nodo->chave < pai->chave)
			pai->fe = nodo;
		else
			pai->fd = nodo;
	}
	// corrige o balanceamento da arvore a partir do pai.
	corrigirBalanco(raiz, pai);
	return nodo;
}

struct nodo *buscar(struct nodo *nodo, int chave)
{
	if (!nodo || nodo->chave == chave)
		return nodo;

	if (chave < nodo->chave)
		return buscar(nodo->fe, chave);

	return buscar(nodo->fd, chave);
}

void transplantar(struct nodo **raiz, struct nodo *x, struct nodo *y)
{
	if (!x->pai) {
		*raiz = y;
	}
	else {
		if (x == x->pai->fe)
			x->pai->fe = y;
		else
			x->pai->fd = y;
	}
	if (y)
		y->pai = x->pai;
}

struct nodo *maximo(struct nodo *nodo)
{
	while (nodo->fd)
		nodo = nodo->fd;

	return nodo;
}

int excluir(struct nodo **raiz, int chave)
{
	struct nodo *nodo = buscar(*raiz, chave);
	struct nodo *pai = NULL;

	if (!nodo)
		return 0;

	if (!nodo->fe && !nodo->fd) { // nodo e uma folha.
		if (nodo == (*raiz)) { // a raiz e uma folha.
			free(nodo);
			*raiz = NULL;
			return 1;
		}
		pai = nodo->pai;
		if (pai->fe == nodo) // folha e filho esquerdo.
			nodo->pai->fe = NULL;
		else // folha e filho direito.
			nodo->pai->fd = NULL;
		free(nodo);
	}
	else if (!nodo->fe) { // nodo possui um filho direito.
		pai = nodo->pai;
		transplantar(raiz, nodo, nodo->fd);
		free(nodo);
	}
	else if (!nodo->fd) { // nodo possui um filho esquerdo.
		pai = nodo->pai;
		transplantar(raiz, nodo, nodo->fe);
		free(nodo);
	}
	else { // nodo possui dois filhos.
		struct nodo *antecessor = maximo(nodo->fe);
		if (antecessor != nodo->fe) { // antecessor nao e filho esquerdo do nodo que sera removido.
			pai = antecessor->pai; // corrige o balanceamento a partir do pai do antecessor.
			transplantar(raiz, antecessor, antecessor->fe);
			antecessor->fe = nodo->fe;
			antecessor->fe->pai = antecessor;
		}
		if(!pai)
			pai = antecessor; // corrige o balanceamento a partir do antecessor.

		transplantar(raiz, nodo, antecessor);
		antecessor->fd = nodo->fd;
		antecessor->fd->pai = antecessor;
		free(nodo);
	}
	// corrige o balanceamento da arvore a partir do pai.
	corrigirBalanco(raiz, pai);
	return 1;
}

void auxImprimirEmOrdem(struct nodo *nodo)
{
	if (nodo) {
		auxImprimirEmOrdem(nodo->fe);
		printf("%d ", nodo->chave);
		auxImprimirEmOrdem(nodo->fd);
	}
}

void imprimirEmOrdem(struct nodo *raiz)
{
	if (raiz) {
		auxImprimirEmOrdem(raiz);
		printf("\n");
	}
}

// retorna o nivel de um nodo.
int nivel(struct nodo *nodo)
{
	int n = 0;

	while (nodo->pai) {
		n++;
		nodo = nodo->pai;
	}
	return n;
}

void imprimirEmLargura(struct nodo *raiz)
{
	if (raiz) {
		struct fila *fila = criaFila();

		if (!fila)
			return;

		int nivelAtual = 0;

		printf("[%d] %d(%d) ", nivelAtual, raiz->chave, balanco(raiz));
		if (raiz->fe)
			enfileirar(fila, raiz->fe);
		if (raiz->fd)
			enfileirar(fila, raiz->fd);

		while (!vaziaFila(fila)) {
			struct nodo *aux = removerCabeca(fila);
			int n = nivel(aux);

			if (n > nivelAtual) {
				printf("\n[%d] %d(%d) ", n, aux->chave, balanco(aux));
				nivelAtual = n;
			}
			else {
				printf("%d(%d) ", aux->chave, balanco(aux));
			}
			if (aux->fe)
				enfileirar(fila, aux->fe);
			if (aux->fd)
				enfileirar(fila, aux->fd);
		}
		destroiFila(fila);
		printf("\n");
	}
}

void destroiArvore(struct nodo *nodo)
{
	if (nodo) {
		destroiArvore(nodo->fe);
		destroiArvore(nodo->fd);
		free(nodo);
	}
}
