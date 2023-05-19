CC = gcc
CFLAGS = -std=c99 -Wall -g

all: backup

backup: backup.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f backup

purge: clean
	find . -type f ! -name 'backup.c' ! -name 'makefile' ! -name 'bkp.bin' -delete
