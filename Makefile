# voxl engine

CC      = gcc
CFLAGS  = -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wno-unused-parameter -O2 -g -Isrc -Ithird_party -Ithird_party/glad/include
LDFLAGS = -lglfw -lGL -lm -ldl -lpthread

SRC := third_party/glad/src/gl.c $(shell find src -name "*.c" -not -name "*_test.c")

OBJ := $(SRC:.c=.o)

BIN := voxl

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

run: $(BIN)
	./$(BIN)

.PHONY: all clean run
