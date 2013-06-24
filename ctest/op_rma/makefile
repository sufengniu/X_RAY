CC=gcc
CFLAGS=-g

all: main

main: main.o
	$(CC) $(CFLAGS) main.o -o main -lm

main.o: main.c sys_config.h
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm *.o main
