CC=gcc
CFLAGS=-Wall

build: fileio

test-fileio: fileio test-fileio.sh
	./test-fileio.sh

test-iozone: test-iozone.sh
	./test-iozone.sh

fileio: fileio.c
	$(CC) $(CFLAGS) -o fileio $<

clean:
	rm -rf fileio

