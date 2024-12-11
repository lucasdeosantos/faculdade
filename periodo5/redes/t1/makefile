# ========================================================================
# Authored by: Lucas Emanuel de Oliveira Santos & Thiago Fabricio de Mello
# Contact: lucasde@ufpr.br & thiago.mello@ufpr.br
# Date: 05/12/2024
# ========================================================================

CC = gcc
CFLAGS = -Wall -lm -g

CLIENT_EXEC = client
SERVER_EXEC = server

CLIENT_SRC = client.c socket.c package.c
SERVER_SRC = server.c socket.c package.c

CLIENT_OBJS = client.o socket.o package.o
SERVER_OBJS = server.o socket.o package.o

HEADERS = socket.h package.h

all:
	@echo "Para rodar como client use: make client"
	@echo "Para rodar como server use: make server"

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_EXEC): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $(CLIENT_EXEC) $(CLIENT_OBJS)

$(SERVER_EXEC): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $(SERVER_EXEC) $(SERVER_OBJS)

clean:
	rm -f *.o $(CLIENT_EXEC) $(SERVER_EXEC)
