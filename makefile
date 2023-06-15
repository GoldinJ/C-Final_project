CC = gcc
CFLAGS = -ansi -pedantic -Wall

all: parser

parser: parser.o linkedlist.o
	$(CC) $(CFLAGS) -o parser parser.o linkedlist.o

parser.o: parser.c constants.h linkedlist.h
	$(CC) $(CFLAGS) -c parser.c 

linkedlist.o: linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist.c 

clean:
	rm -f parser debug *.o