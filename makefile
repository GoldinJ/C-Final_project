CC = gcc
CFLAGS = -ansi -pedantic -Wall

all: assembler

assembler: assembler.o encoder.o linkedlist.o parser.o utilities.o hashtable.o
	$(CC) $(CFLAGS) -o assembler  assembler.o encoder.o linkedlist.o parser.o utilities.o hashtable.o

assembler.o: assembler.c assembler.h
	$(CC) $(CFLAGS) -c assembler.c

encoder: encoder.o linkedlist.o parser.o utilities.o
	$(CC) $(CFLAGS) -o encoder encoder.o linkedlist.o parser.o utilities.o

encoder.o: encoder.c encoder.h 
	$(CC) $(CFLAGS) -c encoder.c

parser: parser.o linkedlist.o
	$(CC) $(CFLAGS) -o parser parser.o linkedlist.o

parser.o: parser.c constants.h linkedlist.h errors.h parser.h
	$(CC) $(CFLAGS) -c parser.c 

linkedlist.o: linkedlist.c utilities.o linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist.c

hashtable.o: hashtable.c hashtable.h
	$(CC) $(CFLAGS) -c hashtable.c  

utilities.o: utilities.c
	$(CC) $(CFLAGS) -c utilities.c 

debug: CFLAGS += -g
debug: clean encoder parser

run:
	./assembler ps

leak-check:
	valgrind --leak-check=full --track-origins=yes ./assembler ps

clean:
	rm -f assembler encoder parser debug *.o
