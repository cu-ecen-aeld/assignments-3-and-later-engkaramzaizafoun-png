# Compiler (force cross compiler)
CC = $(CROSS_COMPILE)gcc

CFLAGS := -Wall -Werror

SRC := aesdsocket.c
TARGET := aesdsocket

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
