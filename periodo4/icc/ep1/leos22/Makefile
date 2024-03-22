# Nome do programa executável
PROG = perfSL

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -Werror -g -O0

# Arquivos fonte
SRCS = gs.c eg.c sl.c utils.c main.c

# Arquivos objeto
OBJS = $(SRCS:.c=.o)

# Regras
all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

purge:
	@echo "Limpando tudo ..."
	@rm -f $(PROG) $(OBJS)

.PHONY: all purge

