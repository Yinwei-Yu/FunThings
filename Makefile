all: cache

cache: cache.o
	gcc -g -o cache cache.c

cache.o: cache.c
	gcc -c -o cache.o cache.c

clean:
	rm -f cache cache.o