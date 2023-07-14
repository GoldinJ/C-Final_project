#include "macro.h"

void expand_macros(FILE *fsrc, char* filename){
    FILE *fdst;
    char *line;
    char *line_copy;
    char **instruction;
    char *macro_name = NULL;
    char **macro_body = NULL;
    char **tmp = NULL;
    int macro_line_cnt = 0;
    int is_macro = FALSE;
    int i;

    HashTable *macro_table = createHashTable();
    char *_filename = malloc(strlen(filename) +4);

    if(_filename == NULL){
        fprintf(stderr, "expand_macros: Memory allocation failed");
        return;
    }

    strcpy(_filename, filename);

    fdst = fopen(strcat(_filename, ".am"), "w");

    if(fdst == NULL){
        fprintf(stderr, "Failed to create %s.as file\n", filename);
        free(_filename);
        return;
    }

    while((line = get_line(fsrc)) != NULL){

        if(line[0] == '\0'){
            continue;
        }

        line_copy = duplicateString(line);
        instruction = parse_command(line_copy);
        
        if(is_macro == FALSE){
            
            if(strcmp(instruction[0], "mcro") == 0){
                is_macro = TRUE;
                macro_name = duplicateString(instruction[1]); /*check if NULL*/
                macro_body = (char**)malloc(sizeof(char*));
                insert(macro_table, macro_name, macro_body);

            }
            else if((tmp = get(macro_table, instruction[0])) != 0){

                for(i=0; tmp[i] != NULL; i++){
                    fprintf(fdst,"%s\n", tmp[i]);
                }

                tmp = NULL;

            }
            else{
                fprintf(fdst, "%s\n", line);
            
            }

        }
        else{

            if(strcmp(instruction[0], "endmcro") == 0){
                is_macro = FALSE;
                macro_body = get(macro_table, macro_name);
                appendString(macro_body, &macro_line_cnt, NULL);
                macro_line_cnt = 0;
                free(macro_name);
            }
            else{
                macro_body = get(macro_table, macro_name);
                appendString(macro_body, &macro_line_cnt, line);
            }

        }
        
        free(line);
        free(line_copy);
        free_command(instruction);

    }

    free(_filename);
    freeHashtableStrings(macro_table);
    fclose(fdst);
}

void checkout_macros(char* filename){
    FILE *fsrc;
    char *full_path = malloc(strlen(filename)+4);

    if(full_path == NULL){
        fprintf(stderr, "checkout_macros: Memory allocation failed");
        return;
    }

    strcpy(full_path, filename);

    fsrc = fopen(strcat(full_path, ".as"), "r");

    if(fsrc == NULL){
        fprintf(stderr, FILE_NOT_FOUND, full_path);
        free(full_path);
        return;
    }

    expand_macros(fsrc, filename);

    fclose(fsrc);
    free(full_path);
}

/* int main(){

    char *filename = "ps";
    checkout_macros(filename);

    return 0;

} */