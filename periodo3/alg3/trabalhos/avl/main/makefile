CC = gcc
CFLAGS = -Wall -g
nomePrograma = trab1_20224379_20224758

objects = main.o avl.o fila.o

all: $(nomePrograma)

$(nomePrograma): $(objects)
	$(CC) -o $(nomePrograma) $(objects)

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

avl.o: avl.h avl.c
	$(CC) -c $(CFLAGS) avl.c

fila.o: fila.h fila.c
	$(CC) -c $(CFLAGS) fila.c

clean:
	rm -f *.o *.gch $(nomePrograma)
