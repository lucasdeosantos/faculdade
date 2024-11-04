CC = gcc
CFLAGS = -Wall -O3

SRC = chrono.c partA.c partB.c
OBJ = $(SRC:.c=.o)

EXEC_PARTA = partA
EXEC_PARTB = partB

all: $(EXEC_PARTA) $(EXEC_PARTB)

# Compilação dos executáveis
$(EXEC_PARTA): partA.o chrono.o
	$(CC) $(CFLAGS) -o $@ $^

$(EXEC_PARTB): partB.o chrono.o
	$(CC) $(CFLAGS) -o $@ $^

# Compilação dos arquivos objeto
%.o: %.c chrono.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(EXEC_PARTA) $(EXEC_PARTB)

.PHONY: all clean
