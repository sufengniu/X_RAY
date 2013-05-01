# Makefile for dark average operation
# Engineer: Sufeng Niu
# Company: Argonne National Lab

CC=gcc
CFLAGS=-g
OBJECT=mthreads_dark_avg

.PHONY: all tar clean

all: $(OBJECT)

$(OBJECT): $(OBJECT).o tif.o avg.o
	$(CC) $(CFLAGS) -pthread $(OBJECT).o tif.o avg.o -o $(OBJECT) -lm -ltiff -lrt

$(OBJECT).o: $(OBJECT).c tif.h avg.h sys_config.h
	$(CC) $(CFLAGS) -c $(OBJECT).c -o $(OBJECT).o

tif.o: tif.c tif.h sys_config.h
	$(CC) $(CFLAGS) -c tif.c -o tif.o

avg.o: avg.c avg.h sys_config.h
	$(CC) $(CFLAGS) -c avg.c -o avg.o

tar: 
	tar cvf *.c *.h makefile

clean:
	rm -rf *.o $(OBJECT) output.tif *.tar
