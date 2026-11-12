# Nome do executável final
TARGET = trab

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -g -pthread

# Arquivos de origem
SOURCES = $(TARGET).c

# Arquivos objeto (.o) baseados nos arquivos fonte
OBJECTS = $(SOURCES:.c=.o)

# Regra padrão (o que o 'make' faz sozinho)
all: $(TARGET)

# Regra para vincular (linkar) o executável
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Regra para compilar o arquivo .c para .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar arquivos gerados
clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
