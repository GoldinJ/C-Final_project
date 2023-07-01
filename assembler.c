#include "assembler.h"

int process_word_queue(HashTable* symbol_table, LinkedList *list, char **instruction){
    int i;
    int dec_address = 100;
    machine_w* tmp;
    machine_w** word_queue = encode(instruction);

    if(word_queue == NULL)
        return FALSE;

    else{

        i = 0;
        while(word_queue[i] != NULL){
            tmp = word_queue[i];
            
            if (tmp->label !=NULL){
                insert(symbol_table, tmp->label, (void*)(long)dec_address);
            }

            if (i == 0)
                add_node(list, instruction, tmp);
            else
                add_node(list, NULL, tmp);
            i++;
            dec_address++;
        }
    }

    return TRUE;
}

void first_pass(FILE *fptr, HashTable *symbol_table, char **externals, char **entries){
    char *line;
    char *line_copy;
    char **instruction = NULL;
    int line_len;
    machine_w** word_queue;

    LinkedList list;
    list.head = NULL;
    list.tail = NULL;
   
    symbol_table = createHashTable();

    while ((line = get_line(fptr)) != NULL) {

        if(line[0] == '\0' || !validate_syntax(line)){
            free(line);
            continue;
        }

        line_len = strlen(line);
        line_copy = malloc((line_len + 1) * sizeof(char));
        strcpy(line_copy, line);
        instruction = parse_command(line_copy);
        word_queue = encode(instruction);
        
        if(process_word_queue(symbol_table, &list, instruction))
            free(word_queue);
        
        
        free(line_copy);
        free(line);
    } 


}

void test(FILE *fptr){
    
    char *line;
    char *line_copy;
    char **instruction = NULL;
    int line_len;
    char *label;
    int i;
    int dec_address = 100;
    machine_w* tmp;
    machine_w** word_queue;
    HashTable* symbol_table = NULL;
    

    LinkedList list;
    Node *tmp_node;
    Node *current_node;
    list.head = NULL;
    list.tail = NULL;
   
    symbol_table = createHashTable(); 

    while ((line = get_line(fptr)) != NULL) {

        if(line[0] == '\0' || !validate_syntax(line)){
            free(line);
            continue;
        }

        line_len = strlen(line);
        line_copy = malloc((line_len + 1) * sizeof(char));
        strcpy(line_copy, line);
        instruction = parse_command(line_copy);
        word_queue = encode(instruction);
        
        if (word_queue == NULL){
            free(line_copy);
            free(line);
            continue;
        }

        i = 0;
        while(word_queue[i] != NULL){
            tmp = word_queue[i];
            
            if (tmp->label !=NULL){
                insert(symbol_table, tmp->label, (void*)(long)dec_address);
            }

            if (i == 0)
                add_node(&list, instruction, tmp);
            else
                add_node(&list, NULL, tmp);
            i++;
            dec_address++;
        }
        
        free(word_queue);
        free(line_copy);
        free(line);
    }

    current_node = list.head;

    while (current_node != NULL)
    {
        tmp_node = current_node;
        current_node = current_node->next;

        if (tmp_node->word->placeholder)
        {
            tmp_node->word->word.im_drct_w->ARE = R;
            label = tmp_node->word->label;
            tmp_node->word->word.im_drct_w->src_operand = (int)(long)get(symbol_table, label);
        }
        
    }
    

    print_list(&list, FALSE);
    freeHashtable(symbol_table);
    free_list(&list);


}

int main(int argc, char *argv[]){
    FILE *fsrc;
    char *fullpath = NULL;
    char **entries = NULL;
    char **externals = NULL;
    int i;

    if(argc == 1){
        printf("No file detected.\n");
        exit(1);
    }

    if (argc > 1){
        for(i = 1; i<argc; i++){
            
            fullpath = strcat(argv[i], ".as");
            fsrc = fopen(fullpath, "r");

            if (fsrc == NULL){
                printf("File not found - %s\n", fullpath);
                exit(1);
            }
            else
                test(fsrc);

        }

    }
    

    return 0;
}