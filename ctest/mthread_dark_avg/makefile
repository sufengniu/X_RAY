# Makefile for dark average operation
# Engineer: Sufeng Niu
# Company: Argonne National Lab

CC=gcc
CFLAGS=-g
OBJECT=mthreads_dark_avg
COMP=avg_2

.PHONY: all tar clean

all: $(OBJECT)

$(OBJECT): $(OBJECT).o tif.o $(COMP).o
	$(CC) $(CFLAGS) -lpthread $(OBJECT).o tif.o $(COMP).o -o $(OBJECT) -lm -ltiff -lrt

$(OBJECT).o: $(OBJECT).c tif.h $(COMP).h sys_config.h
	$(CC) $(CFLAGS) -c $(OBJECT).c -o $(OBJECT).o

tif.o: tif.c tif.h sys_config.h
	$(CC) $(CFLAGS) -c tif.c -o tif.o

$(COMP).o: $(COMP).c $(COMP).h sys_config.h
	$(CC) $(CFLAGS) -c $(COMP).c -o $(COMP).o

tar: 
	tar cvf *.c *.h makefile

clean:
	rm -rf *.o $(OBJECT) dark_avg.tif *.tar
