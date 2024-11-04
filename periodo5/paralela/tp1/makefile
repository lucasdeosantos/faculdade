# Variáveis
CC = gcc
CFLAGS = -Wall -O2

# Arquivos de origem e objetos
SRC_CHRONO = chrono.c
OBJ_CHRONO = chrono.o
SRC_PARTA = partA.c
SRC_PARTB = partB.c
OBJ_PARTA = partA.o
OBJ_PARTB = partB.o

# Executáveis
EXEC_PARTA = partA
EXEC_PARTB = partB

# Regra padrão
all: $(EXEC_PARTA) $(EXEC_PARTB)

# Compilação dos executáveis
$(EXEC_PARTA): $(OBJ_PARTA) $(OBJ_CHRONO)
	$(CC) $(CFLAGS) -o $@ $^

$(EXEC_PARTB): $(OBJ_PARTB) $(OBJ_CHRONO)
	$(CC) $(CFLAGS) -o $@ $^

# Compilação dos arquivos objeto
$(OBJ_CHRONO): $(SRC_CHRONO) chrono.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_PARTA): $(SRC_PARTA) chrono.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_PARTB): $(SRC_PARTB) chrono.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Limpeza dos arquivos objeto e executáveis
clean:
	rm -f $(OBJ_PARTA) $(OBJ_PARTB) $(OBJ_CHRONO) $(EXEC_PARTA) $(EXEC_PARTB)

.PHONY: all clean

