CC      = gcc
CFLAGS  = -g -Wall -std=c99

all: hash bubblesort parallel

hash bubblesort: % : %.c
	$(CC) $(CFLAGS) -o $@ $^

parallel: sort : %.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(OBJS) mytr myzip myunzip

