CC = gcc
CFLAGS = -Wall -g

objects = main.o
	
all: rle

rle: main.o
	$(CC) -o rle $(objects)

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

clean:
	rm -f $(objects) rle
