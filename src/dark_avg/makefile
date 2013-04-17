CC=mpicc
CFLAGS=-g
OBJECT=dark_avg

.PHONY: all tar clean

all: $(OBJECT)

$(OBJECT): $(OBJECT).o tif_access.o
	$(CC) $(CFLAGS) $(OBJECT).o -o $(OBJECT) -lm

$(OBJECT).o: $(OBJECT).c
	$(CC) $(CFLAGS) -c $(OBJECT).c -o $(OBJECT).o

tif_access.o: tif_access.c tif_access.h
	$(CC) $(CFLAGS) -c tif_access.c -o tif_access.o -ltiff

tar: 
	tar cxvf *.c *.h makefile

clean:
	rm -rf *.o $(OBJECT) output.tif
