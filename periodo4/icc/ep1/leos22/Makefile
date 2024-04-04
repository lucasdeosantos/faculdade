PROG = perfSL

CC = gcc

LIKWID_HOME = /home/soft/likwid
CFLAGS = -Wall -Wextra -Werror -g -O0 -I$(LIKWID_HOME)/include -DLIKWID_PERFMON
LFLAGS = -lm -L$(LIKWID_HOME)/lib -llikwid

SRCDIRS = ./src
SOURCE = $(wildcard $(SRCDIRS)/*.c)
OBJS = $(patsubst $(SRCDIRS)/%.c, $(SRCDIRS)/%.o, $(SOURCE))

.PHONY: all purge

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

%.o: $(SRCDIRS)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

purge:
	@echo "Cleaning everything ..."
	@rm -f $(PROG) $(OBJS)

