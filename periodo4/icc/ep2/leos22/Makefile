PROG = matmult

CC = gcc -Wall

LIKWID_HOME = /home/soft/likwid

CFLAGS = -O3 -mavx2 -march=native -I$(LIKWID_HOME)/include -DLIKWID_PERFMON
LFLAGS = -L$(LIKWID_HOME)/lib -llikwid -lm

DISTFILES = Makefile perfctr 
# README
DISTDIR = `basename ${PWD}`

SRCDIRS = ./src
SOURCE = $(wildcard $(SRCDIRS)/*.c)
OBJS = $(patsubst $(SRCDIRS)/%.c, $(SRCDIRS)/%.o, $(SOURCE))

.PHONY: all debug clean purge dist

%.o: $(SRCDIRS)/%.c
	$(CC) $(CFLAGS) -c $<

all: $(PROG)

debug:         CFLAGS += -g -D_DEBUG_
debug:         $(PROG)

$(PROG): $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	@echo "Limpando ...."
	@rm -f *~ *.bak *.tmp core 

purge:   clean
	@echo "Faxina ...."
	@rm -f  $(PROG) $(OBJS) a.out $(DISTDIR) $(DISTDIR).tar
	@rm -f *.png marker.out

dist: purge
	@echo "Gerando arquivo de distribuição ($(DISTDIR).tar) ..."
	@ln -s . $(DISTDIR)
	@tar -cvf $(DISTDIR).tar $(addprefix ./$(DISTDIR)/, $(DISTFILES))
	@rm -f $(DISTDIR)

