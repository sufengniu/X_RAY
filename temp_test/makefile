CC=gcc
CFLAGS=-O3

all: read

read: read.o
	$(CC) $(CFLAGS) read.o -o read -ltiff -lm

read.o: read.c
	$(CC) $(CFLAGS) -c read.c -o read.o

tar: 
	tar cxvf *.c *.h makefile

clean:
	rm -rf *.o read output.tif
