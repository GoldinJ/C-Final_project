CC = gcc
CFLAGS = -ansi -pedantic -Wall

all: utilities

utilities: utilities.o
	$(CC) $(CFLAGS) -o utilities utilities.o

utilities.o: utilities.c 
	$(CC) $(CFLAGS) -c utilities.c 


clean:
	rm -f utilities *.o