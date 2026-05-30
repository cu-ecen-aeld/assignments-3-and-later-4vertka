SHELL = /usr/bin/env bash
.PHONY: all clean

CC ?= gcc
CCFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -g

all: writer

writer: writer.o
	$(CC) $(CCFLAGS) -o writer writer.o

clean:
	rm -f writer writer.o
