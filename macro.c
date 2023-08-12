#include "macro.h"

void expand_macros(FILE *fsrc, char* filename){
    FILE *fdst;
    Macro mcro;
    char *line;
    char *line_copy;
    char **instruction;
    char **tmp = NULL;
    int is_macro = FALSE;
    int i;

    HashTable *macro_table = createHashTable();
    
    fdst = open_file(filename, ".am", "w");

    if(fdst == NULL){
        fprintf(stderr, "Failed to create %s.as file\n", filename);
        return;
    }

    while((line = get_line(fsrc)) != NULL){

        if(line[0] == '\0'){
            free(line);
            continue;
        }

        clean_whitespaces(line);
        line_copy = duplicateString(line);
        instruction = parse_command(line_copy);
        
        if(is_macro == FALSE){
            
            if(strcmp(instruction[0], "mcro") == 0){
                is_macro = TRUE;
                mcro.name = duplicateString(instruction[1]); /*check if NULL*/
                mcro.body = NULL;
                mcro.line_cnt = 0;
                insert(macro_table, mcro.name, mcro.body);

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
                appendString(&(mcro.body), &(mcro.line_cnt), NULL);
                insert(macro_table, mcro.name, mcro.body);
                free(mcro.name);
            }
            else{
                appendString(&(mcro.body), &(mcro.line_cnt), line);
            }

        }
        
        free(line);
        free(line_copy);
        free_command(instruction);
    }

    freeHashtableStrings(macro_table);
    fclose(fdst);
}

int checkout_macros(char* filename){
    FILE *fsrc = NULL;

    fsrc = open_file(filename, ".as", "r");

    if(fsrc == NULL){
        return FALSE;
    }

    expand_macros(fsrc, filename);

    fclose(fsrc);
    return TRUE;
}

/* int main(){

    char *filename = "ps";
    checkout_macros(filename);

    return 0;

} */