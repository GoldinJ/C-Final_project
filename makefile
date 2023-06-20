CC = gcc
CFLAGS = -ansi -pedantic -Wall

all: encoder

encoder: encoder.o linkedlist.o parser.o utilities.o
	$(CC) $(CFLAGS) -o encoder encoder.o linkedlist.o parser.o utilities.o

encoder.o: encoder.c constants.h errors.h linkedlist.h parser.h utilities.h
	$(CC) $(CFLAGS) -c encoder.c

parser: parser.o linkedlist.o
	$(CC) $(CFLAGS) -o parser parser.o linkedlist.o

parser.o: parser.c constants.h linkedlist.h errors.h
	$(CC) $(CFLAGS) -c parser.c 

linkedlist.o: linkedlist.c utilities.o linkedlist.h utilities.h
	$(CC) $(CFLAGS) -c linkedlist.c 

utilities.o: utilities.c
	$(CC) $(CFLAGS) -c utilities.c 

debug: CFLAGS += -g
debug: clean encoder parser

clean:
	rm -f encoder parser debug *.o
