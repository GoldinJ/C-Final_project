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
        if(i == 0 && !IS_CHAR(line[0]) && line[0] != '.'){
            printf(ILLEGAL_CHARACTER_IN_LABEL, line[0]);
            return FALSE;
        }


        else if (line[i] == ',') {
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



char** Order_array(FILE *fptr, int* numRows) {
    const int MAX_ROWS = 100;  // Maximum number of rows allowed

    char** orderArray = malloc(MAX_ROWS * sizeof(char*));
    int i = 0;  // Rows

    char line[MAX_LINE_LEN];
    while (fgets(line, MAX_LINE_LEN, fptr) != NULL && i < MAX_ROWS) {
        // Remove newline character if present
        size_t lineLength = strlen(line);
        if (lineLength > 0 && line[lineLength - 1] == '\n') {
            line[lineLength - 1] = '\0';
            lineLength--;
        }

        orderArray[i] = malloc((lineLength + 1) * sizeof(char));
        strcpy(orderArray[i], line);
        i++;
    }

    *numRows = i;

    return orderArray;
}

void free_orderArray(char** orderArray) {
    int i;
    for (i = 0; orderArray[i] != NULL; i++) {
        free(orderArray[i]);
    }
    free(orderArray);
}

/*Function for finding the name of the macro and storing it an array and saving the macro body into a 2D array corresponding to the name */

MacroData FindMacroData(char** orderArray) {
    int i = 0;
    int count = 0;
    char** macro_name = NULL;
    char*** macro_body = NULL;
    int* macro_body_counts = NULL;
    char** current_macro_body = NULL;
    MacroData macro_data;

    while (orderArray[i] != NULL) {
        if (strcmp(orderArray[i], "mcro") == 0) {
            if (orderArray[i + 1] != NULL) {
                i += 2; /*move the i to point at the macro body*/

                /*allocate memory for the macro names*/
                macro_name = realloc(macro_name, (count + 1) * sizeof(char*));
                if (macro_name == NULL) {
                    fprintf(stderr, "\"macro_name: Memory allocation failed\"\n");
                    exit(1);
                }
                macro_name[count] = malloc((strlen(orderArray[i - 1]) + 1) * sizeof(char));
                if (macro_name[count] == NULL) {
                    fprintf(stderr, "\"macro_name: Memory allocation failed\"\n");
                    exit(1);
                }
                strcpy(macro_name[count], orderArray[i - 1]);

                /*allocate memory for the macro bodies*/
                macro_body_counts = realloc(macro_body_counts, (count + 1) * sizeof(int));
                if (macro_body_counts == NULL) {
                    fprintf(stderr, "\"macro_body_counts: Memory allocation failed\"\n");
                    exit(1);
                }
                macro_body_counts[count] = 0;

                /*processing macro_body */
                while (orderArray[i] != NULL && strcmp(orderArray[i], "endmcro") != 0) {
                    /* Allocate memory for current_macro_body*/
                    current_macro_body = realloc(current_macro_body, (macro_body_counts[count] + 1) * sizeof(char*));
                    if (current_macro_body == NULL) {
                        fprintf(stderr, "\"current_macro_body: Memory allocation failed\"\n");
                        exit(1);
                    }
                    current_macro_body[macro_body_counts[count]] = malloc((strlen(orderArray[i]) + 1) * sizeof(char));
                    if (current_macro_body[macro_body_counts[count]] == NULL) {
                        fprintf(stderr, "\"current_macro_body: Memory allocation failed\"\n");
                        exit(1);
                    }
                    strcpy(current_macro_body[macro_body_counts[count]], orderArray[i]);
                    i++;
                    macro_body_counts[count]++;
                }

                /*allocate memory for macro body*/
                macro_body = realloc(macro_body, (count + 1) * sizeof(char**));
                if (macro_body == NULL) {
                    fprintf(stderr, "\"macro_body: Memory allocation failed\"\n");
                    exit(1);
                }
                macro_body[count] = current_macro_body;
                count++;
            }
        }
        i++;
    }

    macro_name = realloc(macro_name, (count + 1) * sizeof(char*));
    if (macro_name == NULL) {
        fprintf(stderr, "\"macro_name: Memory allocation failed\"\n");
        exit(1);
    }
    macro_name[count] = NULL;

    /* Create MacroData structure to hold the name, body, and count of the lines */
    macro_data.macro_names = macro_name;
    macro_data.macro_bodies = macro_body;
    macro_data.macro_body_counts = macro_body_counts;

    return macro_data;
}




void MacroLayout(char** orderArray, MacroData macro_data) {
    FILE* outputFile = fopen("output.am", "w");

    if (outputFile == NULL) {
        fprintf(stderr, "Failed to create the output file.\n");
        return;
    }

    for (int i = 0; orderArray[i] != NULL; i++) {
        if (strcmp(orderArray[i], "mcro") == 0) {
            char* macro_name = orderArray[i + 1];
            int j = 0;
            int macro_index = -1;

            while (macro_data.macro_names[j] != NULL) {
                if (strcmp(macro_data.macro_names[j], macro_name) == 0) {
                    macro_index = j;
                    break;
                }
                j++;
            }

            if (macro_index != -1) {
                int k = 0;
                while (macro_data.macro_bodies[macro_index][k] != NULL) {
                    fprintf(outputFile, "%s\n", macro_data.macro_bodies[macro_index][k]);
                    k++;
                }
            } else {
                i++;
            }
        } else {
            fprintf(outputFile, "%s\n", orderArray[i]);
        }
    }

    fclose(outputFile);
}




void FreeMacroData(MacroData macroData) {
    /* Free macro names */
    int i, j;
    if (macroData.macro_names != NULL) {
        for (i = 0; macroData.macro_names[i] != NULL; i++) {
            free(macroData.macro_names[i]);
        }
        free(macroData.macro_names);
    }

    /* Free macro bodies */
    if (macroData.macro_bodies != NULL) {
        for (i = 0; macroData.macro_bodies[i] != NULL; i++) {
            char** macroBody = macroData.macro_bodies[i];
            for (j = 0; macroBody[j] != NULL; j++) {
                free(macroBody[j]);
            }
            free(macroBody);
        }
        free(macroData.macro_bodies);
    }

    /* Free macro body counts */
    free(macroData.macro_body_counts);
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