#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"

#define TABLE_SIZE 100
#define MAX_ARRAY_SIZE 100

typedef struct Entry {
    char* key;
    void* value;
    struct Entry* next;
} Entry;

typedef struct {
    Entry** entries;
} HashTable;

unsigned int hash(const char* key);

char* duplicateString(const char* source);

Entry* createEntry(const char* key, void* value);

HashTable* createHashTable();

void insert(HashTable* table, const char* key, void* value);

void* get(HashTable* table, const char* key);

void removeEntry(HashTable* table, const char* key);

void printHashTable(HashTable* table); 

void freeHashtable(HashTable* table);

#endif /* HASH_TABLE_H */

