# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -std=c99 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = testa_conjunto.o lib_conjunto.o tp4.o
     
all: tp4.o lib_conjunto.o
	$(CC) -o tp4 tp4.o lib_conjunto.o $(LDFLAGS)

lib_conjunto.o: lib_conjunto.c
	$(CC) -c $(CFLAGS) lib_conjunto.c

tp4.o: tp4.c
	$(CC) -c $(CFLAGS) tp4.c

clean:
	rm -f $(objects) tp4


