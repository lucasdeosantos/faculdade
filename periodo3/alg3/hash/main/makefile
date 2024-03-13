parametrosCompilacao=-Wall #-Wshadow
nomePrograma=trab2_20224379

all: $(nomePrograma)

$(nomePrograma): main.o tabelaHash.o
	gcc -o $(nomePrograma) main.o tabelaHash.o $(parametrosCompilacao)

main.o: main.c
	gcc -c main.c $(parametrosCompilacao)

tabelaHash.o: tabelaHash.h tabelaHash.c
	gcc -c tabelaHash.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)
