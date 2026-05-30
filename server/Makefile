SHELL=/bin/bash

CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS =
TARGET = aesdsocket 
SRCS = aesdsocket.c

all default: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
