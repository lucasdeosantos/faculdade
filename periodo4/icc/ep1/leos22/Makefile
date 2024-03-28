PROG = perfSL

CC = gcc

LIKWID_HOME = /home/soft/likwid
CFLAGS = -Wall -Wextra -Werror -g -O0 -I$(LIKWID_HOME)/include -DLIKIWD_PERFMON
LFLAGS = -L$(LIKWID_HOME)/lib -llikwid

SRCS = gs.c ge.c ls.c utils.c main.c

OBJS = $(SRCS:.c=.o)

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

purge:
	@echo "Limpando tudo ..."
	@rm -f $(PROG) $(OBJS)

.PHONY: all purge

