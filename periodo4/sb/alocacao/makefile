CC = gcc
CFLAGS = -g -no-pie

AS = as
ASFLAGS = -g

SRCDIR = ./src

all: main

main: main.o memalloc.o
	$(CC) $(CFLAGS) main.o memalloc.o -o main

memalloc.o: $(SRCDIR)/memalloc.s
	$(AS) $(ASFLAGS) $(SRCDIR)/memalloc.s -o memalloc.o

memalloc_c.o: $(SRCDIR)/memalloc.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/memalloc.c -o memalloc_c.o

main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/main.c -o main.o

clean:
	rm -rf ./*.o

purge: clean
	rm -rf main

usec: main.o memalloc_c.o
	$(CC) $(CFLAGS) main.o memalloc_c.o -o main
