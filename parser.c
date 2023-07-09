#include "constants.h"
#include "parser.h"
#include "errors.h"

void strip(char *str) {
    int len = strlen(str);
    int start = 0, end = len - 1;
    
    while (isspace(str[start])) {
        start++;
    }
    
    while (isspace(str[end])) {
        end--;
    }
    
    /* if the string is all spaces/tabs, set it to an empty string*/
    if (end < start) {
        str[0] = '\0';
    } else {
        /*move the non-space/tab characters to the beginning of the string*/ 
        memmove(str, str+start, end-start+1);
        str[end-start+1] = '\0';
    }
}

char* get_line(FILE *fptr){

    int cnt=0;
    char c;
    int line_len = 20;
    char* line = malloc(line_len*sizeof(char));
 
    if (line == NULL){
        printf("get_line: Memory allocation failed");
        exit(1);
    }

    c  = fgetc(fptr);

    if(c == '\n'){
        line[cnt] = '\0';
        return line;
    }

    if (c == ';') {
        while ((c = fgetc(fptr)) != '\n' && !feof(fptr)) {
            /*Skip the current line*/
        }
        line[cnt] = '\0';
        return line;
    }

    ungetc(c, fptr);

    while ((c=fgetc(fptr)) != '\n' && !feof(fptr))
    {
        if(line_len == MAX_LINE_LEN){
            printf("The command is to long, max allowed command length is %d charecters\n", MAX_LINE_LEN);
            exit(1);
        }

        if(cnt == line_len){
            line_len += 10;
            line = realloc(line, line_len*sizeof(char));

            if (line == NULL){
                printf("get_line: Memory allocation failed");
                exit(1);
            }

        }

        line[cnt++] = c;

    }

    if (cnt == 0 && feof(fptr)) {
        /* no more input lines, return NULL */
        free(line);
        return NULL;
    }

    line[cnt] = '\0'; /* add null terminator to the end of the line */
    strip(line);
    return line;
    
}

int validate_syntax(char* line) {
    int i;
    int len = strlen(line);
    int commaCount = 0;
    int operandExpected = TRUE;

    for (i = 0; i < len; i++) {
        if (i == 0 && !IS_CHAR(line[0]) && line[0] != '.') {
            printf(ILLEGAL_CHARACTER_IN_LABEL, line[0]);
            return FALSE;
        } else if (line[i] == ',') {
            if (operandExpected || i == 0 || line[i - 1] == ',') {
                return FALSE;  /* Comma in an unexpected position */
            }
            commaCount++;
            operandExpected = TRUE;

        } else {
            operandExpected = FALSE;
        }
    }

    if (operandExpected) {
        return FALSE;  /* Last token is an operand */
    }

    return TRUE;
}



char** parse_command(char* line){
    
    int i=0;
    char **command = malloc(INSTR_SIZE * sizeof(char *));

    char *token = strtok(line, " \t, \t");

    while (token != NULL) {

        command[i] = malloc((strlen(token) + 1) * sizeof(char));
        strcpy(command[i], token);
        /* printf("+ parse_command - %p, '%s'\n", command[i], command[i]); */
        i++;
        token = strtok(NULL, " \t, \t");
    }
    command[i] = NULL;

    return command;
}


MacroData NameBodyExtractor(FILE* fptr) {
    MacroData macro_data;
    int macro_count = 0;
    int line_count = 0;
    char* line;
    char** macro_names = malloc(10 * sizeof(char*));
    char*** macro_bodies = malloc(10 * sizeof(char**));
    int* macro_body_counts = malloc(10 * sizeof(int));

    if (macro_names == NULL || macro_bodies == NULL || macro_body_counts == NULL) {
        printf("Order_array: Memory allocation failed");
        exit(1);
    }

    while ((line = get_line(fptr)) != NULL) {
        if (strncmp(line, "mcro", 4) == 0) {
            /* Extract macro name */
            char* macro_name = malloc((strlen(line) - 4 + 1) * sizeof(char));
            strcpy(macro_name, line + 4);

            /* Initialize macro body*/
            int body_count = 0;
            char** macro_body = malloc(10 * sizeof(char*));

            if (macro_name == NULL || macro_body == NULL) {
                printf("Order_array: Memory allocation failed");
                exit(1);
            }

            /* Read macro body until "endmcro" is encountered */
            while ((line = get_line(fptr)) != NULL && strcmp(line, "endmcro") != 0) {
                macro_body[body_count] = malloc((strlen(line) + 1) * sizeof(char));
                strcpy(macro_body[body_count], line);
                body_count++;

                /* Resize macro_body array if necessary*/
                if (body_count % 10 == 0) {
                    macro_body = realloc(macro_body, (body_count + 10) * sizeof(char*));
                    if (macro_body == NULL) {
                        printf("Order_array: Memory allocation failed");
                        exit(1);
                    }
                }
            }

            macro_body[body_count] = NULL;  // Add NULL terminator to the macro body array

            /* Store macro name, body, and count in MacroData*/
            macro_names[macro_count] = macro_name;
            macro_bodies[macro_count] = macro_body;
            macro_body_counts[macro_count] = body_count;

            macro_count++;

            /* Resize arrays if necessary*/
            if (macro_count % 10 == 0) {
                macro_names = realloc(macro_names, (macro_count + 10) * sizeof(char*));
                macro_bodies = realloc(macro_bodies, (macro_count + 10) * sizeof(char**));
                macro_body_counts = realloc(macro_body_counts, (macro_count + 10) * sizeof(int));

                if (macro_names == NULL || macro_bodies == NULL || macro_body_counts == NULL) {
                    printf("Order_array: Memory allocation failed");
                    exit(1);
                }
            }
        }
        line_count++;
        free(line);
    }

    macro_names[macro_count] = NULL;     // Add NULL terminator to the macro name array
    macro_bodies[macro_count] = NULL;    // Add NULL terminator to the macro body array
    macro_body_counts[macro_count] = -1; // Set the count of the last macro to -1

    macro_data.macro_names = macro_names;
    macro_data.macro_bodies = macro_bodies;
    macro_data.macro_body_counts = macro_body_counts;

    return macro_data;
}



void freeNameAndBody(MacroData macro_data) {
    int i,j;
    for ( i = 0; macro_data.macro_names[i] != NULL; i++) {
        for ( j = 0; macro_data.macro_bodies[i][j] != NULL; j++) {
            free(macro_data.macro_bodies[i][j]);
        }
        free(macro_data.macro_bodies[i]);
        free(macro_data.macro_names[i]);
    }
    free(macro_data.macro_bodies);
    free(macro_data.macro_names);
    free(macro_data.macro_body_counts);
}


/*int main (){

    char *line;
    char *line_copy;
    char **instruction = NULL;
    int line_len;
    int i = 0;

    LinkedList list;
    list.head = NULL;
    list.tail = NULL;

    while ((line = get_line()) != NULL) {
        line_len = strlen(line);
        line_copy = malloc((line_len + 1) * sizeof(char));
        strcpy(line_copy, line);

        if (!validate_syntax(line_copy)) {

            free(line_copy);
            free(line);
            continue;
        }

        instruction = parse_command(line_copy);
        i = 0;
        printf("%s ------ ", line);
        while (instruction[i] != NULL)
        {
            printf("%d | ", get_token_type(instruction[i]));
            i++;
        }
        printf("\n++++++++++++++++++++++++++++++++\n");
        
        add_node(&list, instruction);
        free(line_copy);
        free(line);
    }

    print_list(&list, FALSE);
    free_list(&list);


    return 0;
*/