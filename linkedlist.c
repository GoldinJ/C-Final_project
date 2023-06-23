#include "linkedlist.h"

void add_node(LinkedList* lst, char** instruction, machine_w* word) {

    Node* newNode = (Node*)(malloc(sizeof(Node)));

    if (word == NULL){
        free(newNode);
        return;
    }

    newNode->instruction = instruction;
    newNode->word = word;
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
        
        if(temp->instruction != NULL){
            /* Free the instruction */
            for (i = 0; temp->instruction[i] != NULL; i++) {
                free(temp->instruction[i]);
            }
            free(temp->instruction);
        }

        if (temp->word != NULL) {
            if (temp->word->label != NULL) {
                free(temp->word->label);
            }

            switch (temp->word->node_type)
            {
            case NODE_FIRST_W:
                free(temp->word->word.f_word); 
                break;
            case NODE_IMDT_DRCT_W:
                free(temp->word->word.im_drct_w); 
                break;
            case NODE_REG_W:
                free(temp->word->word.r_word); 
                break;
            case NODE_DATA_W:
                free(temp->word->word.d_word); 
                break;

            default:
                break;
            }
            
            free(temp->word);
        }

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
        
        if(temp->instruction != NULL){
            printf("---------------\n");
        /* Print the instructions */
        for (i = 0; temp->instruction[i] != NULL; i++) {
            printf("%s", temp->instruction[i]);
            
            /* Print a separator if there is another instruction */
            if (temp->instruction[i + 1] != NULL) {
                printf("|");
            }
        }
        printf("\n");
        }
        print_last_12_bits(*((unsigned int *)(temp->word->word.f_word)));
        /* printf("=================================\n"); */
        
        current = current->next;
    }
}
