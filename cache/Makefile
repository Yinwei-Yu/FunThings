all: cache

CC=gcc
CFALGS=-g -Wall

b=4
s=8
c=32

cache: cache.o
	$(CC) $(CFALGS) -o cache cache.c

cache.o: cache.c
	$(CC) -c -o cache.o cache.c

clean:
	rm -f cache cache.o