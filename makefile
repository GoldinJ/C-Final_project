CC = gcc
CFLAGS = -ansi -pedantic -Wall

all: assembler

assembler: assembler.o encoder.o linkedlist.o parser.o utilities.o hashtable.o macro.o errors.o
	$(CC) $(CFLAGS) -o assembler  assembler.o encoder.o linkedlist.o parser.o utilities.o hashtable.o macro.o errors.o

assembler.o: assembler.c assembler.h
	$(CC) $(CFLAGS) -c assembler.c

errors: errors.o
	$(CC) $(CFLAGS) -o errors errors.o

errors.o: errors.c errors.h
	$(CC) $(CFLAGS) -c errors.c

encoder: encoder.o linkedlist.o parser.o utilities.o
	$(CC) $(CFLAGS) -o encoder encoder.o linkedlist.o parser.o utilities.o

encoder.o: encoder.c encoder.h 
	$(CC) $(CFLAGS) -c encoder.c

macro: macro.o parser.o hashtable.o utilities.o
	$(CC) $(CFLAGS) -o macro macro.o parser.o hashtable.o utilities.o

macro.o: macro.c macro.h 
	$(CC) $(CFLAGS) -c macro.c

parser: parser.o parser.h
	$(CC) $(CFLAGS) -o parser parser.o

parser.o: parser.c 
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
	rm -f assembler encoder parser macro debug errors *.o *.ent *.ext *.ob *.am
