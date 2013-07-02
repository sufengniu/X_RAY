CC=mpicc
CFLAGS=-g

all: main

main: main.o
	$(CC) $(CFLAGS) main.o dark_mode.o thr_pool.o -o main -lm

main.o: main.c sys_config.h
	$(CC) $(CFLAGS) -c main.c -o main.o

dark_mode.o: dark_mode.c dark_mode.h udp_server.h tif.h rms.h thr_pool.h sys_config.h
	$(CC) $(CFLAGS) -c dark_mode.c -o dark_mode.o

thr_pool.o: thr_pool.c thr_pool.h
	$(CC) $(CFLAGS) -c thr_pool.c -o thr_pool.o

clean:
	rm -rf *.o main
