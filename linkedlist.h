#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "constants.h"
#define REVERSE 1
#define FORWARD 0

enum node_type {NODE_ENTRY, NODE_EXTERN, NODE_FIRST_W, NODE_IMDT_DRCT_W, NODE_REG_W, NODE_DATA_W};

typedef struct Node {
    char ** instruction;
    machine_w* word;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct LinkedList {
    Node* head;
    Node* tail;
} LinkedList;

void add_node(LinkedList* lst, char** instruction, machine_w* word);
void print_node(char **instruction);
void print_list(LinkedList* lst, int reverse);
void print_to_file(FILE *fptr, LinkedList *lst, int reverse);
void free_list(LinkedList* lst);


#endif 
