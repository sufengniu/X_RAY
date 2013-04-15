CC=g++
CFLAGS=-g

all: main

main: tinytiff.o main.o
	$(CC) $(CFLAGS) tinytiff.o main.o -o main

tinytiff.o: tinytiff.cpp tinytiff.h
	$(CC) $(CFLAGS) -c tinytiff.cpp -o tinytiff.o

main.o: tinytiff.h main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

clean:
	rm -rf *.o main output.tif
