CC = gcc
CFLAGS = -Wall -O3

SRC = multi_partition.c chrono.c verifica_particoes.c
OBJ = $(SRC:.c=.o)

EXEC = multi_partition

all: $(EXEC)

$(EXEC): multi_partition.o chrono.o verifica_particoes.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c chrono.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
