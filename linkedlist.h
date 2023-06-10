#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#define REVERSE 1
#define FORWARD 0

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    unsigned long value;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct LinkedList {
    Node* head;
    Node* tail;
} LinkedList;

void add_node(LinkedList* lst, int val);
void print_list(LinkedList* lst, int reverse);
void print_to_file(FILE *fptr, LinkedList *lst, int reverse);
void free_list(LinkedList* lst);

#endif 
