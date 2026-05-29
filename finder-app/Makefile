SHELL = /usr/bin/env bash
.PHONY: clean

CC ?= gcc
CCFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -g

writer: writer.o
	$(CC) $(CCFLAGS) -o writer writer.o

clean:
	rm -f writer writer.o
