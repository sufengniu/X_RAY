CC=mpicc
CFLAGS=-O3

all: main

main: main.o
	$(CC) $(CFLAGS) main.o -o main -ltiff -lm

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

tar: 
	tar cxvf *.c *.h makefile

clean:
	rm -rf *.o scanline scanstrip scantile output.tif
