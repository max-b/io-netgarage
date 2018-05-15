# LIBS = -lm
TARGETS = level02 level03 level04
LIBS = 
CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra -Werror -Werror-implicit-function-declaration
BUILD_DIR = ./build/
BINS = $(addprefix $(BUILD_DIR), $(TARGETS))

.PHONY: default all clean

default: $(TARGETS)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
		$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS) $(TARGETS)

$(TARGETS): $(OBJECTS)
		$(CC) $@.o $(CFLAGS) $(LIBS) -o $(addprefix $(BUILD_DIR), $@)

clean:
		-rm -f *.o
		-rm -f $(BINS)
