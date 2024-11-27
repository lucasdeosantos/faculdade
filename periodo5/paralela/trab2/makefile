CC = gcc
CFLAGS = -Wall -O3

SRC = chrono.c multi_partition.c
OBJ = $(SRC:.c=.o)

EXEC = multi_partition

all: $(EXEC)

$(EXEC): multi_partition.o chrono.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c chrono.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
