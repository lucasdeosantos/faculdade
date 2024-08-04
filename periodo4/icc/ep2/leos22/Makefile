PROG = matmult

CC = gcc -Wall

LIKWID_HOME = /home/soft/likwid

CFLAGS = -O3 -mavx2 -march=native -I$(LIKWID_HOME)/include -DLIKWID_PERFMON
LFLAGS = -L$(LIKWID_HOME)/lib -llikwid -lm

DISTFILES = Makefile gendata.sh genplot.py plot.gp myplot.gp README
DISTDIR = `basename ${PWD}`

RESTDIR = ./Resultados

SRCDIR = ./src
SOURCE = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(SRCDIR)/%.o, $(SOURCE))

.PHONY: all debug clean purge dist

%.o: $(SRCDIR)/%.c
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
	@rm -r $(RESTDIR)

dist: purge
	@echo "Gerando arquivo de distribuição ($(DISTDIR).tar) ..."
	@ln -s . $(DISTDIR)
	@tar -cvf $(DISTDIR).tar $(addprefix ./$(DISTDIR)/, $(DISTFILES))
	@rm -f $(DISTDIR)

