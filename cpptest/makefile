CC=g++
CFLGAS=-g -I.

all: quickstart

quickstart: quickstart.o
	$(CC) $(CFLAGS) $^ -o $@ -I.

quickstart.o: quickstart.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o quickstart
