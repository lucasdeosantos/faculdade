parametrosCompilacao=-Wall #-Wshadow
nomePrograma=rainhas

all: $(nomePrograma)

$(nomePrograma): main.o pilha.o
	gcc -o $(nomePrograma) main.o pilha.o $(parametrosCompilacao)

main.o: main.c
	gcc -c main.c $(parametrosCompilacao)

pilha.o: pilha.h pilha.c
	gcc -c pilha.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)
