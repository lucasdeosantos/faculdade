TARGET = mpc

SRC = src

CC = gcc
CFLAGS = -Wall -g -I$(SRC)

SRCS = $(SRC)/parser.tab.c $(SRC)/lex.yy.c $(SRC)/symbols_table.c $(SRC)/main.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

$(SRC)/parser.tab.c $(SRC)/parser.tab.h: $(SRC)/parser.y
	bison -d -o $(SRC)/parser.tab.c $(SRC)/parser.y

$(SRC)/lex.yy.c: $(SRC)/lexer.l $(SRC)/parser.tab.h
	flex -o $(SRC)/lex.yy.c $(SRC)/lexer.l

clean:
	rm -f $(TARGET) $(SRC)/parser.tab.c $(SRC)/parser.tab.h $(SRC)/lex.yy.c

.PHONY: all clean