# -------------------------------------------------------------------
# Makefile for aesdsocket.c with cross-compilation support
# -------------------------------------------------------------------
# Compiler
CC ?= gcc
# If CROSS_COMPILE is set, use it as prefix for compiler
ifdef CROSS_COMPILE
CC := $(CROSS_COMPILE)gcc
endif

# Compiler flags
CFLAGS := -Wall -Werror

# Source and target
SRC := aesdsocket.c
TARGET := aesdsocket

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

# Clean target
clean:
	rm -f $(TARGET) *.o

# Phony targets
.PHONY: all clean
