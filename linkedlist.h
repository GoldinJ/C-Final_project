#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "constants.h"
#include "utilities.h"

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
void make_obj_file(LinkedList* lst, char* filename);


#endif 
