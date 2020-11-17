CC      = gcc
CFLAGS  = -g -Wall -std=c99

all: hash bubblesort parallel

hash bubblesort parallel: % : %.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(OBJS) hash bubblesort parallel

