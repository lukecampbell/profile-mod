CC=gcc
LDFLAGS=-lm -lrt
CFLAGS=-g -ggdb

all: bin/mods

bin:
	mkdir bin

bin/mods: src/main.o bin
	$(CC) -o $@ $< $(LDFLAGS) 

src/main.o: src/main.c
	$(CC) -o $@ $(CFLAGS) -c $^
