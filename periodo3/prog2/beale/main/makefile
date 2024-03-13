CC = gcc
CFLAGS = -std=c99 -Wall -g

objects = beale.o codificacao.o decodificacao.o cifras.o lista.o
	
all: beale

beale: $(objects)
	$(CC) -o beale $(objects)

beale.o: beale.c beale.h
	$(CC) -c $(CFLAGS) beale.c

codificacao.o: codificacao.c beale.h
	$(CC) -c $(CFLAGS) codificacao.c

decodificacao.o: decodificacao.c beale.h
	$(CC) -c $(CFLAGS) decodificacao.c

cifras.o: cifras.c beale.h
	$(CC) -c $(CFLAGS) cifras.c

lista.o: lista.c beale.h
	$(CC) -c $(CFLAGS) lista.c

clean:
	rm -f $(objects) beale

purge: clean
	find . -type f ! -name '*.c' ! -name '*.h' ! -name 'LEIAME.txt' ! -name 'makefile' -delete
