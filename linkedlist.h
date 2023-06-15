#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#define REVERSE 1
#define FORWARD 0

typedef struct Node {
    char ** instruction;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct LinkedList {
    Node* head;
    Node* tail;
} LinkedList;

void add_node(LinkedList* lst, char** instruction);
void print_node(char **instruction);
void print_list(LinkedList* lst, int reverse);
void print_to_file(FILE *fptr, LinkedList *lst, int reverse);
void free_list(LinkedList* lst);


#endif 
