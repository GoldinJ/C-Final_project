#include "assembler.h"

int process_word_queue(HashTable* symbol_table, LinkedList *list, char **instruction, int *dec_address){
    int i;
    machine_w* tmp;
    machine_w** word_queue = encode(instruction);

    if(word_queue == NULL)
        return FALSE;

    else{

        i = 0;
        while(word_queue[i] != NULL){
            tmp = word_queue[i];
            
            if (tmp->label !=NULL){
                insert(symbol_table, tmp->label, (void*)(long)(*dec_address));
            }

            if (i == 0)
                add_node(list, instruction, tmp);
            else
                add_node(list, NULL, tmp);
            i++;
            (*dec_address)++;
        }
    }

    free(word_queue);
    return TRUE;
}

int process_symbols(char** instruction, HashTable *external_symbols, HashTable *entry_symbols){
    int i = 0;
    /* char *label; */
    int is_ext = FALSE;
    int is_ent = FALSE;
    
    if(instruction == NULL)
        return FALSE;

    while(instruction[i] != NULL){
        /* if(get_token_type(instruction[i]) == TOKEN_LABEL_DEFENITION)
            label = instruction[i]; */

        if(get_token_type(instruction[i]) == TOKEN_EXTERN){
            is_ext = TRUE;
        }

        else if(get_token_type(instruction[i]) == TOKEN_ENTRY){
            is_ent = TRUE;
        }

        else if (get_token_type(instruction[i]) == TOKEN_LABEL && is_ent)
        {
            insert(entry_symbols, instruction[i], (void*)(long)R);
        }
        else if (get_token_type(instruction[i]) == TOKEN_LABEL && is_ext)
        {
            insert(external_symbols, instruction[i], (void*)(long)E);
           
        }
        
        i++;
    }
    
    if(is_ent || is_ext){
        free_command(instruction);
        instruction = NULL;
        return FALSE;
    }

    return TRUE;
}

void first_pass(FILE *fptr, LinkedList *list, HashTable *symbol_table, HashTable *external_symbols, HashTable *entry_symbols){
    char *line;
    char *line_copy;
    char **instruction = NULL;
    int line_len;
    int dec_address = 100;

    while ((line = get_line(fptr)) != NULL) {

        if(line[0] == '\0' || !validate_syntax(line)){
            free(line);
            continue;
        }

        line_len = strlen(line);
        line_copy = malloc((line_len + 1) * sizeof(char));
        strcpy(line_copy, line);
        instruction = parse_command(line_copy);
    
        if(process_symbols(instruction, external_symbols, entry_symbols))
            process_word_queue(symbol_table, list, instruction, &dec_address);

        free(line_copy);
        free(line);
    }
    
    if(fptr != NULL)
        fclose(fptr);
}

void second_pass(char *filename, LinkedList *list, HashTable *symbol_table, HashTable *external_symbols, HashTable *entry_symbols, int *IC, int *DC){
    
    int cnt = 0;
    char *label;
    FILE *fentry = NULL;
    FILE *fextern = NULL;
    Node *tmp_node;
    Node *current_node = list->head;
    char *fname_entry = NULL;
    char *fname_ext = NULL;

    while (current_node != NULL)
    {
        tmp_node = current_node;
        current_node = current_node->next;
        label = tmp_node->word->label;

        if(tmp_node->word->node_type == NODE_DATA_W)
            (*DC)++;
        else
            (*IC)++;

        if(label != NULL && !tmp_node->word->placeholder){
            if ((int)(long)(get(entry_symbols, tmp_node->word->label) != 0)){
                if(fentry == NULL){
                    fname_entry = duplicateString(filename);
                    fentry = fopen(strcat(fname_entry, ".ent"), "w");
                }
                fprintf(fentry, "%s\t%d\n", tmp_node->word->label, (int)(long)(get(symbol_table, tmp_node->word->label)));
            }
        }

        if (tmp_node->word->placeholder)
        {

            if ((int)(long)(get(external_symbols, label) != 0)){
                tmp_node->word->word.im_drct_w->ARE = E;
                tmp_node->word->word.im_drct_w->src_operand = 0;

                if(fextern == NULL){
                    fname_ext = duplicateString(filename);
                    fextern = fopen(strcat(fname_ext, ".ext"), "w");
                }
                    
                fprintf(fextern, "%s\t%d\n", label, cnt+100);
                cnt++;
                continue;
                    
            }
            
            tmp_node->word->word.im_drct_w->ARE = R;
            tmp_node->word->word.im_drct_w->src_operand = (int)(long)get(symbol_table, label);
        }

        
        cnt++;
        
    }

    if (fentry!=NULL)
        fclose(fentry);
    if (fextern!=NULL)
        fclose(fextern);
    if (fname_entry != NULL)
        free(fname_entry);
    if (fname_ext != NULL)
        free(fname_ext);
        
}

void make_obj_file(LinkedList* lst, char* filename, int IC, int DC){
    FILE *fobj = NULL;
    Node* current = lst->head;
    Node* temp = NULL;
    char *ob_filename;
    char *base64;

     if(fobj == NULL){
            ob_filename = duplicateString(filename);
            fobj = fopen(strcat(ob_filename, ".ob"), "w");
            fprintf(fobj, "%d %d\n", IC, DC);
        }

    while (current != NULL)
    {
        temp = current;
        current = current->next;
        base64 = int_to_Base64(*((unsigned int *)temp->word->word.f_word));
        fprintf(fobj, "%s\n", base64);
        free(base64);
    }
    
    if(fobj != NULL){
        free(ob_filename);
        fclose(fobj);
    }

}

void process_input(FILE *fptr, char* filename){
    
    int IC = 0;
    int DC = 0;
    HashTable* symbol_table =  createHashTable();
    HashTable* external_symbols = createHashTable();
    HashTable* entry_symbols = createHashTable();
    
    LinkedList list;
    list.head = NULL;
    list.tail = NULL;

    first_pass(fptr, &list, symbol_table, external_symbols, entry_symbols);
    second_pass(filename, &list, symbol_table, external_symbols, entry_symbols, &IC, &DC);

    printf("Extern symbols:\n----------------------\n");
    printHashTable(external_symbols);
    printf("Entry symbols:\n----------------------\n");
    printHashTable(entry_symbols);
    printf("Symbols table:\n----------------------\n");
    printHashTable(symbol_table);
    /* print_list(&list, FALSE); */
    make_obj_file(&list, filename, IC, DC);

    freeHashtable(symbol_table);
    freeHashtable(entry_symbols);
    freeHashtable(external_symbols);
    free_list(&list);

}

void encoder_use_case(FILE *fptr){
    
    char *line;
    char *line_copy;
    char **instruction = NULL;
    int line_len;
    int i;
    machine_w* tmp;
    machine_w** word_queue;

    LinkedList list;
    list.head = NULL;
    list.tail = NULL;

    while ((line = get_line(fptr)) != NULL) {
        if (!validate_syntax(line)) {
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
            if (i == 0)
                add_node(&list, instruction, tmp);
            else
                add_node(&list, NULL, tmp);
            i++;
        }
        free(word_queue);
        free(line_copy);
        free(line);
    }

    print_list(&list, FALSE);
    free_list(&list);
    fclose(fptr);

}

int main(int argc, char *argv[]){
    FILE *fsrc;
    char *fullpath = NULL;
    char *file_name = NULL;
    int i;

    if(argc == 1){
        printf("No file detected.\n");
        exit(1);
    }

    if (argc > 1){
        for(i = 1; i<argc; i++){
            
            file_name = duplicateString(argv[i]);
            fullpath = strcat(argv[i], ".am");
            fsrc = fopen(fullpath, "r");

            if (fsrc == NULL){
                printf("File not found - %s\n", fullpath);
                continue;
            }
            else
                process_input(fsrc, file_name);

            free(file_name);
        }

    }
    

    return 0;
}