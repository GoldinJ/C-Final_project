CC = gcc
CFLAGS = -ansi -pedantic -Wall

all: encoder

encoder: encoder.o linkedlist.o parser.o
	$(CC) $(CFLAGS) -o encoder encoder.o linkedlist.o parser.o

encoder.o: encoder.c constants.h errors.h linkedlist.h parser.h
	$(CC) $(CFLAGS) -c encoder.c

parser: parser.o linkedlist.o
	$(CC) $(CFLAGS) -o parser parser.o linkedlist.o

parser.o: parser.c constants.h linkedlist.h errors.h
	$(CC) $(CFLAGS) -c parser.c 

linkedlist.o: linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist.c 

debug: CFLAGS += -g
debug: clean encoder parser

clean:
	rm -f encoder parser debug *.o
