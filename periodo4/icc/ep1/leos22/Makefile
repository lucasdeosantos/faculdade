PROG = perfSL

CC = gcc
OBJS = gs.o ge.o ls.o utils.o main.o

LIKWID_HOME = /home/soft/likwid
CFLAGS = -Wall -Wextra -Werror -g -O0 -I$(LIKWID_HOME)/include -DLIKIWD_PERFMON
LFLAGS = -L$(LIKWID_HOME)/lib -llikwid

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all purge

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LFLAGS)

purge:
	@echo "Cleaning everything ..."
	@rm -f $(PROG) $(OBJS)

