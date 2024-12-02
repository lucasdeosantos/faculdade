CC = gcc
CFLAGS = -O3 -pthread

SRC = chrono.c parteA.c parteB.c
OBJ = $(SRC:.c=.o)

EXEC_PARTEA = parteA
EXEC_PARTEB = parteB

all: $(EXEC_PARTEA) $(EXEC_PARTEB)

# Compilação dos executáveis
$(EXEC_PARTEA): parteA.o chrono.o
	$(CC) $(CFLAGS) -o $@ $^

$(EXEC_PARTEB): parteB.o chrono.o
	$(CC) $(CFLAGS) -o $@ $^

# Compilação dos arquivos objeto
%.o: %.c chrono.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(EXEC_PARTEA) $(EXEC_PARTEB)

.PHONY: all clean
