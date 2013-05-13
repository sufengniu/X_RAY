# Makefile for dark average operation
# Engineer: Sufeng Niu
# Company: Argonne National Lab

CC=gcc
CFLAGS=-g -O3
OBJECT=mthreads_dark_avg

.PHONY: all tar clean

all: $(OBJECT)

$(OBJECT): $(OBJECT).o tif.o avg.o
	$(CC) $(CFLAGS) -lpthread $(OBJECT).o tif.o avg.o -o $(OBJECT) -lm -ltiff -lrt

$(OBJECT).o: $(OBJECT).c tif.h avg.h sys_config.h
	$(CC) $(CFLAGS) -c $(OBJECT).c -o $(OBJECT).o

tif.o: tif.c tif.h sys_config.h
	$(CC) $(CFLAGS) -c tif.c -o tif.o

avg.o: avg.c avg.h sys_config.h
	$(CC) $(CFLAGS) -c avg.c -o avg.o

#thr_pool.o: thr_pool.c thr_pool.h
#	$(CC) $(CFALGS) -c thr_pool.c -o thr_pool.o

tar: 
	tar cvf *.c *.h makefile

clean:
	rm -rf *.o $(OBJECT) dark_avg.tif *.tar
