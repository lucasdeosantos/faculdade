Lucas Emanuel de Oliveira Santos 20224379

Para a implementação da Tabela Hash foi utilizado um vetor de nodos hash, um nodo hash contém uma chave e ponteiros para o nodo anterior e o próximo nodo da lista. A função hash é dada por k mod m, onde k é a chave e m é o tamanho da Tabela Hash. Quando a funçao recebe uma chave negativa, transforma ela em positiva e em seguida calcula k mod m.

A funcão inserir() recebe uma chave k, cria e insere um nodo na Tabela Hash. A função calcula o índice de k no vetor através da função hash() e testa se o ponteiro daquele índice é nulo(lista está vazia), se for nulo, adiciona no ínicio da lista de elementos daquele índice, do contrário, insere no final da lista. Para não aceitar duplicadas, enquanto a função percorre a lista, testa se k é igual a alguma das chaves dos nodos.

A funcão buscar() recebe uma chave k e retorna o nodo correspondente aquela chave se existir ou nulo caso contrário. A função calcula o índice de k no vetor através da função hash(), se o ponteiro do índice for nulo(lista está vazia) retorna nulo, do contrário, passa por todos os elementos da lista comparando k com a chave do nodo, se a chave do nodo for igual a k retorna o nodo, se nenhuma chave da lista for igual a k retorna nulo.

Antes da exclusão é feita uma busca pelo elemento, se existir, realiza a remoção, do contrário, o nodo não existe na Tabela Hash.

A função excluir() recebe um nodo e o remove da Tabela Hash. Existem quatro casos na remoção:

Caso 1: ambos os ponteiros são nulos, ou seja, a lista possui apenas um elemento.
	Libera a memória alocada para o nodo e o ponteiro do índice apontar para nulo.

Caso 2: o ponteiro para o anterior é nulo, mas o ponteiro para o próximo não é, ou seja, a lista possui mais de um elemento e o nodo é o primeiro elemento da lista.
	Libera a memória alocada para o nodo, o anterior do próximo aponta para nulo e o ponteiro do indice aponta para o próximo.
	
Caso 3: o ponteiro para o anterior não é nulo, mas o ponteiro para o próximo é, ou seja, a lista possui mais de um elemento e o nodo é o último elemento da lista.
	Libera a memória alocada para o nodo, o próximo do anterior aponta para nulo.

Caso 4: ambos os ponteiros são diferentes de nulo, ou seja, a lista possui mais de um elemento e o nodo está entre dois elementos da lista.
	Libera a memória alocada para o nodo, o próximo do anterior aponta para o próximo e anterior do próximo aponta para o anterior.
