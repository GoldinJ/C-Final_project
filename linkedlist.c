#include <stdio.h>
#include <stdlib.h>
#define REVERSE 1
#define FORWARD 0

typedef struct Node {
    unsigned long value;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct LinkedList {
    Node* head;
    Node* tail;
} LinkedList;

void add_node(LinkedList* lst, int val) {
    Node* newNode = (Node*)(malloc(sizeof(Node)));

    newNode->value = val;
    newNode->next = NULL;

    if (lst->head == NULL) {
        newNode->prev = NULL;
        lst->head = newNode;
        lst->tail = newNode;
    } 
    else {
        newNode->prev = lst->tail;
        (lst->tail)->next = newNode;
        lst->tail = newNode;
    }
}

void free_list(LinkedList* lst) {
    Node* p = lst->head;
    Node* nextNode;

    while (p != NULL) {
        nextNode = p->next;
        free(p);
        p = nextNode;
    }

    lst->head = NULL;
    lst->tail = NULL;
}

void print_to_file(FILE *fptr, LinkedList *lst,  int reverse){
    Node* p = (reverse == REVERSE) ? lst->tail : lst->head;

    while (p != NULL) {
        fprintf(fptr, "%lu ", p->value);
        p = (reverse == REVERSE) ? p->prev : p->next;
    }
    fprintf(fptr, "\n");
}

void print_list(LinkedList* lst, int reverse) {

    Node* p = (reverse == REVERSE) ? lst->tail : lst->head;

    while (p != NULL) {
        printf("%lu ", p->value);
        p = (reverse == REVERSE) ? p->prev : p->next;
    }
    putchar('\n');
}