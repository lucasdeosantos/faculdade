CC = gcc

CFLAGS = -Wall -O3

SRC = modeling.c

TARGET = modeling

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
