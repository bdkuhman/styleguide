CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99
#-Werror

.PHONY: all
all: styleguide

styleguide: styleguide.o
	$(CC) $(CFLAGS) styleguide.o -o styleguide

styleguide.o : styleguide.c
	$(CC) $(CFLAGS) -c styleguide.c

.PHONY: clean
clean:
	rm -f styleguide.o styleguide
