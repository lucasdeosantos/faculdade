CC = gcc
CFLAGS = -g

.PHONY: all clean

all: mpc

mpc: lexer.o parser.tab.o main.o ast.o codegen.o symbols_table.o
	$(CC) $(CFLAGS) -o mpc lexer.o parser.tab.o main.o ast.o codegen.o symbols_table.o

lexer.c: lexer.l parser.tab.h
	flex -o lexer.c lexer.l

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lexer.o: lexer.c
	$(CC) $(CFLAGS) -c lexer.c

parser.tab.o: parser.tab.c
	$(CC) $(CFLAGS) -c parser.tab.c

main.o: main.c parser.tab.h
	$(CC) $(CFLAGS) -c main.c

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c

codegen.o: codegen.c codegen.h ast.h symbols_table.h
	$(CC) $(CFLAGS) -c codegen.c

symbols_table.o: symbols_table.c symbols_table.h ast.h
	$(CC) $(CFLAGS) -c symbols_table.c

clean:
	rm -f mpc *.o *.tab.c *.tab.h lexer.c output.ll
