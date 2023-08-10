#include "assembler.h"

int EC = 0;
int LINE_CNT = 0; 

int process_word_queue(HashTable* symbol_table, LinkedList *list, char **instruction, int *dec_address, char* filename){
    int i;
    machine_w* tmp;
    machine_w** word_queue = encode(instruction);

    if(word_queue == NULL){
        EC++;
        fprintf(stderr, ERROR_LOCATION, filename, LINE_CNT);
        return FALSE;
    }

    else{

        i = 0;
        while(word_queue[i] != NULL){
            tmp = word_queue[i];
            
            if (tmp->label !=NULL && (tmp->node_type == NODE_FIRST_W || tmp->node_type == NODE_DATA_W)){
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

int process_symbols(char** instruction, HashTable *external_symbols, HashTable *entry_symbols, char *filename){
    int i = 0;
    int is_ext = FALSE;
    int is_ent = FALSE;
    int token_type = TOKEN_UNDEFINED;
    
    if(instruction == NULL)
        return FALSE;

    while(instruction[i] != NULL){

        token_type = get_token_type(instruction[i]);

        switch (token_type)
        {
            case TOKEN_UNDEFINED:
                free_command(instruction);
                instruction = NULL;
                EC++;
                fprintf(stderr, ERROR_LOCATION, filename, EC);
                return FALSE;
                break;

            case TOKEN_ENTRY:
                is_ent = TRUE;
                break;

            case TOKEN_EXTERN:
                is_ext = TRUE;
                break;

            case TOKEN_LABEL:
                if(is_ent)
                    insert(entry_symbols, instruction[i], (void*)(long)R);
                else if (is_ext)
                    insert(external_symbols, instruction[i], (void*)(long)E);

                break;
                

            default:
                break;
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

int write_label_to_file(FILE **fptr, char* filename, char* extension, char* label, int address){

    if(*fptr == NULL){
        *fptr = open_file(filename, extension, "w");

        if (*fptr == NULL) {
            fprintf(stderr, "Error opening file %s.%s\n", filename, extension);
            return FALSE;
        }
    }

    fprintf(*fptr, "%s\t%d\n", label, address);

    return TRUE;
}

void first_pass(FILE *fptr, char *filename, LinkedList *list, HashTable *symbol_table, HashTable *external_symbols, HashTable *entry_symbols){
    char *line = NULL;
    char *line_copy = NULL;
    char **instruction = NULL;
    int line_len = 0;
    int dec_address = 100;

    LINE_CNT = 0;

    while ((line = get_line(fptr)) != NULL) {
        LINE_CNT++;

        if(line[0] == '\0'){
            free(line);
            continue;
        }

        else if(!error_check(line, filename, LINE_CNT)){
            EC++;
            free(line);
            continue;
        }

        /* else if(!validate_syntax(line)){
            EC++;
            fprintf(stderr, ERROR_LOCATION, filename, LINE_CNT);
            free(line);
            continue;
        } */

        
        line_len = strlen(line);
        line_copy = malloc((line_len + 1) * sizeof(char));
        strcpy(line_copy, line);
        instruction = parse_command(line_copy);
    
        if(process_symbols(instruction, external_symbols, entry_symbols, filename))
            process_word_queue(symbol_table, list, instruction, &dec_address, filename);

        free(line_copy);
        free(line);
    }

}

void second_pass(char *filename, LinkedList *list, HashTable *symbol_table, HashTable *external_symbols, HashTable *entry_symbols, int *IC, int *DC){
    
    int cnt = 0;
    char *label;
    FILE *fentry = NULL;
    FILE *fextern = NULL;
    Node *tmp_node;
    Node *current_node = list->head;

    if(EC > 0)
        return;

    while (current_node != NULL)
    {
        tmp_node = current_node;
        current_node = current_node->next;
        label = tmp_node->word->label;

        if(tmp_node->word->node_type == NODE_DATA_W || tmp_node->word->node_type == NODE_FIRST_DATA_W)
            (*DC)++;
        else
            (*IC)++;

        if(label != NULL){ /*Its a first_w or an imdt_drct_w*/
            
            if(tmp_node->word->placeholder == TRUE){ /*imdt_drct_w*/

                if(get(external_symbols, label) != 0 && get(entry_symbols, label) != 0){
                    EC++;
                    fprintf(stderr, "Error: Label - '%s' - defined both as .extern and as .entry\n", label);
                    break;
                }

                else if(get(external_symbols, label) != 0){
                    
                    tmp_node->word->word.im_drct_w->ARE = E;
                    tmp_node->word->word.im_drct_w->src_operand = 0;

                    if(write_label_to_file(&fextern, filename, ".ext", label, cnt+100) == FALSE){
                        break;
                    } 
    
                }
                else if(get(symbol_table, label) != 0){
                    tmp_node->word->word.im_drct_w->ARE = R;
                    tmp_node->word->word.im_drct_w->src_operand = (int)(long)get(symbol_table, label);
                }

                else if(get(symbol_table, label) == 0){
                    EC++;
                    fprintf(stderr, "Error: Label - '%s' - declaration is missing\n", label);
                    break;
                }

            }
            else if(tmp_node->word->placeholder == FALSE){ /*is a first_w*/

                if(get(entry_symbols, label) != 0 && get(symbol_table, label) != 0){

                    if(write_label_to_file(&fentry, filename, ".ent", label, (int)(long)get(symbol_table, label)) == FALSE){
                        break;
                    }
                }

            }

        }
        cnt++;
        
    }

    if (fentry!=NULL){
        fclose(fentry);
        if(EC>0)
            remove_file(filename, ".ent");
    }
        
    if (fextern!=NULL){
        fclose(fextern);

        if(EC>0)
            remove_file(filename, ".ext");
    }
        
}

void make_obj_file(LinkedList* lst, char* filename, int IC, int DC){
    FILE *fobj = NULL;
    Node* current = lst->head;
    Node* temp = NULL;
    char *base64;

    if(EC > 0){
        printf("Errors (%d errors) are found in %s.am\n", EC, filename);
        printf("%s.ob - wasn't created\n", filename);
        return;
    }

    if(fobj == NULL){
            fobj = open_file(filename, ".ob", "w");
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
        fclose(fobj);
    }

}

void process_input(char* filename){
    
    static int MEMORY_SIZE = 1024;
    int IC = 0;
    int DC = 0;
    FILE *fsrc = NULL;
    HashTable* symbol_table =  createHashTable();
    HashTable* external_symbols = createHashTable();
    HashTable* entry_symbols = createHashTable();
    
    LinkedList list;
    list.head = NULL;
    list.tail = NULL;

    fsrc = open_file(filename, ".am", "r");

    if (fsrc == NULL){
        fprintf(stderr, FILE_NOT_FOUND, filename);
        return;
    }

    first_pass(fsrc, filename, &list, symbol_table, external_symbols, entry_symbols);
    second_pass(filename, &list, symbol_table, external_symbols, entry_symbols, &IC, &DC);
    MEMORY_SIZE = MEMORY_SIZE - IC - DC;

   /*  printf("Extern symbols:\n----------------------\n");
    printHashTable(external_symbols);
    printf("Entry symbols:\n----------------------\n");
    printHashTable(entry_symbols);
    printf("Symbols table:\n----------------------\n");
    printHashTable(symbol_table);
    print_list(&list, FALSE); */

    if(MEMORY_SIZE < 0){
        EC++;
        fprintf(stderr, MEMORY_OVERFLOW);
        remove_file(filename, ".ext");
        remove_file(filename, ".ent");
    }
    make_obj_file(&list, filename, IC, DC);


    freeHashtable(symbol_table);
    freeHashtable(entry_symbols);
    freeHashtable(external_symbols);
    fclose(fsrc);
    free_list(&list);

}

int main(int argc, char *argv[]){
    char *file_name = NULL;
    int i;

    if(argc == 1){
        fprintf(stderr, "No input file detected.\n");
        exit(1);
    }

    if (argc > 1){
        for(i = 1; i<argc; i++){
            
            file_name = duplicateString(argv[i]);
            if(checkout_macros(file_name));
                process_input(file_name);
            free(file_name);
        }

    }
    

    return 0;
}