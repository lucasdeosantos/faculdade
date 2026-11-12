all: chang_roberts randomized

chang_roberts: chang_roberts.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic chang_roberts.o smpl.o rand.o -lm

randomized: randomized.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic randomized.o smpl.o rand.o -lm

smpl.o: smpl.c smpl.h
	$(COMPILE.c)  -g smpl.c

chang_roberts.o: chang_roberts.c smpl.h
	$(COMPILE.c)  -g chang_roberts.c

randomized.o: randomized.c smpl.h
	$(COMPILE.c)  -g randomized.c

rand.o: rand.c
	$(COMPILE.c) -g -std=gnu89 rand.c

clean:
	$(RM) *.o chang_roberts randomized relat saida
