CC=mpicc
CFLAGS=-g
OBJECT=dark_rms

all: $(OBJECT)

$(OBJECT): $(OBJECT).o tif_load.o
	$(CC) $(CFLAGS) $(OBJECT).o -o $(OBJECT) -lm

$(OBJECT).o: $(OBJECT).c
	$(CC) $(CFLAGS) -c $(OBJECT).c -o $(OBJECT).o

tif_load.o: tif_load.c tif_load.h
	$(CC) $(CFLAGS) -c tif_load.c -o tif_load.o -ltiff

tar: 
	tar cxvf *.c *.h makefile

clean:
	rm -rf *.o $(OBJECT) output.tif
