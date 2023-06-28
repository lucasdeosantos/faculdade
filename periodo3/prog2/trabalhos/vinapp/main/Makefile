CC := gcc
CFLAGS := -Wall -Wextra -g

SRCDIRS := ./src
SOURCES := $(wildcard $(SRCDIRS)/*.c)
OBJECTS := $(patsubst $(SRCDIRS)/%.c,$(SRCDIRS)/%.o,$(SOURCES))

TARGET := vina++

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: $(SRCDIRS)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJECTS)
