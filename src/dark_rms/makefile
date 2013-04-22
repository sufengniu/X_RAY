# Makefile for dark root mean squre operation
# Engineer: Sufeng Niu
# Company: Argonne National Lab

CC=mpicc
CFLAGS=-g
OBJECT=dark_rms

.PHONY: all tar clean

all: $(OBJECT

$(OBJECT): $(OBJECT).o tif.o
	$(CC) $(CFLAGS) $(OBJECT).o tif.o -o $(OBJECT) -lm -ltiff

$(OBJECT).o: $(OBJECT).c tif.h
	$(CC) $(CFLAGS) -c $(OBJECT).c -o $(OBJECT).o

tif.o: tif.c tif.h
	$(CC) $(CFLAGS) -c tif.c -o tif.o

tar: 
	tar cvf *.c *.h makefile

clean:
	rm -rf *.o $(OBJECT) output.tif
