

CC=gcc
CFLAGS=-g -Wall

all: udp_server udp_client

udp_server: udp_server.o
	$(CC) $(CFLAGS) udp_server.o -o udp_server

udp_server.o: udp_server.c
	$(CC) $(CFLAGS) -c udp_server.c -o udp_server.o
	
udp_client: udp_client.o
	$(CC) $(CFLAGS) $^ -o $@

udp_client.o: udp_client.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -rf *.o udp_server udp_client
	


