#include "linkedlist.h"

void add_node(LinkedList* lst, char** instruction) {
    Node* newNode = (Node*)(malloc(sizeof(Node)));

    newNode->instruction = instruction;
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

void free_list(LinkedList* list) {
    Node* current = list->head;
    Node* temp = NULL;
    int i;
    while (current != NULL) {
        temp = current;
        current = current->next;
        
        /* Free the instruction */
        for (i = 0; temp->instruction[i] != NULL; i++) {
            free(temp->instruction[i]);
        }
        free(temp->instruction);
        
        /* Free the node */
        free(temp);
    }
    
    /* Reset the head and tail pointers */
    list->head = NULL;
    list->tail = NULL;
}

void print_list(LinkedList* lst, int reverse) {

    Node* current = lst->head;
    Node* temp = NULL;
    int i;
    while (current != NULL) {
        temp = current;
        
        /* Print the instructions */
        for (i = 0; temp->instruction[i] != NULL; i++) {
            printf("%s", temp->instruction[i]);
            
            /* Print a separator if there is another instruction */
            if (temp->instruction[i + 1] != NULL) {
                printf("|");
            }
        }
        printf("\n=================================\n");
        
        current = current->next;
    }
}