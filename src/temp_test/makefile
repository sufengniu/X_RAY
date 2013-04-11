CC=gcc
CFLAGS=-O3

all: read read_line

read: read.o
	$(CC) $(CFLAGS) read.o -o read -ltiff -lm

read.o: read.c
	$(CC) $(CFLAGS) -c read.c -o read.o

read_line: read_line.c
	$(CC) $(CFLAGS) -c read_line.c -o read_line -ltiff

tar: 
	tar cxvf *.c *.h makefile

clean:
	rm -rf *.o read output.tif
