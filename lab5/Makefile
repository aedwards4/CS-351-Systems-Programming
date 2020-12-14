CC=gcc
CFLAGS=-Wall

build: cpubench

test-cpubench: cpubench
	./runbench.sh

cpubench: cpubench.c
	$(CC) $(CFLAGS) -Ofast -pthread -o cpubench  $<

clean:
	rm -rf cpubench

