#include "assembler.h"

void test(FILE *fptr){
    
    char *line;
    char *line_copy;
    char **instruction = NULL;
    int line_len;
    int i;
    int dec_address = 100;
    machine_w* tmp;
    machine_w** word_queue;
    HashTable* label_table = NULL;

    LinkedList list;
    list.head = NULL;
    list.tail = NULL;

    label_table = createHashTable(); 

    while ((line = get_line(fptr)) != NULL) {

        if(line[0] == '\0'){
            free(line);
            continue;
        }

        line_len = strlen(line);
        line_copy = malloc((line_len + 1) * sizeof(char));
        strcpy(line_copy, line);

        if (!validate_syntax(line_copy)) {
            free(line_copy);
            free(line);
            continue;
        }

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
                insert(label_table, tmp->label, (void*)(long)dec_address);
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

    printHashTable(label_table);
    print_list(&list, FALSE);
    freeHashtable(label_table);
    free_list(&list);


}

int main(int argc, char *argv[]){
    FILE *fsrc;
    HashTable *table = NULL;
    char *fullpath = NULL;
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