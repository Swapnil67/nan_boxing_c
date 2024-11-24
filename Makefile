CFLAGS=-Wall -Wextra -Wswitch-enum -Wmissing-prototypes -std=c11 -pedantic
LIBS=

.PHONY: all
all: nan

nan: ./src/main.c ./src/nan.h
	$(CC) $(CFLAGS) -o nan ./src/main.c $(LIBS)