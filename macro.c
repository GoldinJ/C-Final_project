#include "macro.h"

void expand_macros(FILE *fsrc, char* filename){
    FILE *fdst;
    char *line;
    char *line_copy;
    char **instruction;
    char *macro_name = NULL;
    char **macro_body = NULL;
    int is_macro = FALSE;

    HashTable *macro_table = createHashTable();
    char *_filename = duplicateString(filename);
    fdst = fopen(strcat(_filename, ".as", "w"));

    if(fdst == NULL){
        fprintf("Failed to create %s.as file\n", filename);
        free(_filename);
        return
    }

    while((line = get_line(fsrc)) != NULL){

        line_copy = duplicateString(line);
        instruction = parse_command(line_copy);
        
        if(is_macro == FALSE){
            
            if(strcmp(instruction[0], "mcro")){
                is_macro = TRUE;
                macro_name = duplicateString(instruction[1]); /*check if NULL*/
            }
            else if(strcmp(instruction[0], "endmcro")){
                is_macro = FALSE;
                insert(macro_table, macro_name, macro_body);
                free(macro_name);
                free_command(macro_body);
                
            }
            else{
                fprintf(fdst, "%s\n", line);
            
            }

            free(line);
            free(line_copy);
            free_command(instruction);
        }
        else{
            
            /*append incoming lines into macro_body*/
        }
        

    }

    free(_filename);
    fclose(fdst);
}

void checkout_macros(char* filename){
    char *_filename = duplicateString(filename);
    FILE *fsrc;

    fsrc = fopen(strcat(_filename, ".as"), "r");

    if(fsrc == NULL){
        fprintf(FILE_NOT_FOUND, _filename);
        free(_filename);
        return;
    }

    expand_macros(FILE *fsrc, char* filename);

    fclose(fsrc);
}