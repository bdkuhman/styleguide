CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99 
#-Werror

.PHONY: all
all: styleguide

styleguide: styleguide.o
	$(CC) $(CFLAGS) styleguide.o -o styleguide
	
styleguide.o :
	$(CC) $(CFLAGS) styleguide.c -c styleguide.o

.PHONY: clean
clean:
	rm *.o
