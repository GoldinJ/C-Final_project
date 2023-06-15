#include "constants.h"
#include "linkedlist.h"


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

char* get_line(){

    int cnt=0;
    char c;
    int line_len = 0;
    char* line = malloc(line_len*sizeof(char));

    if (line == NULL){
        printf("get_line: Memory allocation failed");
        exit(1);
    }

    if((c=getchar()) == '\n'){
        line[cnt] = '\0';
        return line;
    }

    ungetc(c, stdin);

    while ((c = getchar()) != '\n' && c != EOF)
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

    if (cnt == 0 && c == EOF) {
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
    int operandExpected = 1;

    for (i = 0; i < len; i++) {
        if (line[i] == ' ' || line[i] == '\t') {
            continue;  /* Skip whitespace */
        } else if (line[i] == ',') {
            if (operandExpected || i == 0 || line[i - 1] == ',') {
                return 0;  /* Comma in an unexpected position */
            }
            commaCount++;
            operandExpected = 1;
        } else {
            operandExpected = 0;
        }
    }

    if (operandExpected) {
        return 0;  /* Last token is an operand */
    }

    return 1;
}

char** parse_command(char* line){
    
    int i=0;
    char **command = malloc(INSTR_SIZE * sizeof(char *));

    char *token = strtok(line, " \t, \t");

    while (token != NULL) {

        command[i] = malloc(strlen(token) * sizeof(char));
        strcpy(command[i], token);
        i++;
        token = strtok(NULL, " \t, \t");
    }
    command[i] = NULL;

    return command;
}

void free_command(char** command) {
    int i;
    for (i = 0; command[i] != NULL; i++) {
        free(command[i]); /*free each individual string*/ 
    }
    free(command); /*free the array itself*/ 
}

int main (){

    char *line;
    char *line_copy;
    char **instruction = NULL;
    int line_len;

    LinkedList list;
    list.head = NULL;
    list.tail = NULL;

    while ((line = get_line()) != NULL) {
        line_len = strlen(line);
        line_copy = malloc((line_len + 1) * sizeof(char));
        strcpy(line_copy, line);

        if (!validate_syntax(line_copy)) {
            printf("Error in line %s\n", line_copy);
            free(line_copy);
            free(line);
            continue;
        }

        instruction = parse_command(line_copy);
        add_node(&list, instruction);
        free(line_copy);
        free(line); /* free the memory allocated by get_line */
    }

    print_list(&list, FALSE);
    free_list(&list);

    return 0;
}