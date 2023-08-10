#include "parser.h"

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

void clean_whitespaces(char *str) {
    int i;
    int len = strlen(str);
    int inWhitespace = FALSE;
    int newIndex = 0;

    for (i = 0; i < len; i++) {
        if (str[i] == ' ' || str[i] == '\t') {
            if (!inWhitespace) {
                inWhitespace = TRUE;
                str[newIndex++] = ' ';
            }
        } else if (str[i] == ',') {
            if (newIndex > 0 && str[newIndex - 1] == ' ') {
                str[newIndex - 1] = ','; /*  Remove space before comma */
            } else {
                str[newIndex++] = ','; /*  Add comma */
            }
            str[newIndex++] = ' '; /*  Add single space after comma */
            inWhitespace = TRUE; /*  Set to true to avoid consecutive spaces */
        } else {
            inWhitespace = FALSE;
            str[newIndex++] = str[i];
        }
    }

     if (newIndex > 0 && str[newIndex - 1] == ' ') {
        str[newIndex - 1] = '\0';
    } else {
        str[newIndex] = '\0';
    }

}

char* duplicate_str(const char* source) {
    size_t length;
    char* destination = NULL;

    if(source == NULL)
        return NULL;
        
    length = strlen(source);
    destination = (char*)malloc(length + 1);
    if (destination != NULL) {
        strcpy(destination, source);
    }
    return destination;
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
    int i = 0;
    int len = strlen(line);
    int tokenCount = 0;
    int tokenExpected = TRUE;
    char *endLabel;
   
    if((endLabel=strchr(line, ':')) != NULL){
        i = endLabel - line + 1;
    }
    
    while(i<len){
        
        while(isspace(line[i]))
            i++;
            
        if(line[i] == ','){
            if(tokenExpected)
                return FALSE;
            else if(i == len-1)
                return FALSE;
                
            tokenExpected = TRUE;
        }
        else if(IS_VALID_CHAR(line[i])){
            
            if(!tokenExpected){
                return FALSE;
            }
            
            while(IS_VALID_CHAR(line[i]))
                i++;
                
            tokenCount++;
            tokenExpected = (tokenCount>1)?FALSE:TRUE;
            continue;
        }
        else
            tokenExpected = FALSE;
            

        i++;
        
        
    }

    return TRUE;
}

char** parse_command(char* line) {
    int i = 0;
    int j;
    int len = strlen(line);
    int start = 0; /*  Start index of the current token */
    int state = 0; /*  0: Regular token parsing, 1: Inside quotation marks */

    char **command = malloc(INSTR_SIZE * sizeof(char *));
    /* printf("+ command - %p\n", command); */
    
    if (command == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    for (j = 0; j <= len; j++) {
        if (state == 0 && (line[j] == ' ' || line[j] == '\t' || line[j] == ',' || line[j] == '\0')) {
            if (j > start) { /*  Avoid empty tokens */
                line[j] = '\0'; /*  Null-terminate the token */
                command[i] = duplicate_str(line + start); /*  Copy the token content */
                /* printf("+ parse_command %s - %p\n", command[i], command[i]); */
                if (command[i] == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    free_command(command);
                    return NULL;
                }
                i++;
            }
            start = j + 1;
        } else if (state == 0 && line[j] == '"') {
            state = 1;
            start = j; /*  Include the starting quotation mark */
        } else if (state == 1 && line[j] == '"') {
            line[j + 1] = '\0'; /*  Null-terminate the token at the ending quotation mark */
            command[i] = duplicate_str(line + start); /*  Copy the token content with quotation marks */
            /* printf("+ parse_command%s - %p\n", command[i], command[i]); */
            if (command[i] == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                free_command(command);
                return NULL;
            }
            i++;
            state = 0;
            start = j + 2; /*  Skip the ending quotation mark and the following character */
        }
    }
    
    command[i] = NULL;

    return command;
}

/* char** parse_command(char* line){
    
    int i=0;
    char **command = malloc(INSTR_SIZE * sizeof(char *));

    char *token = strtok(line, " \t, \t");

    while (token != NULL) {

        command[i] = malloc((strlen(token) + 1) * sizeof(char));
        strcpy(command[i], token);
        i++;
        token = strtok(NULL, " \t, \t");
    }
    command[i] = NULL;

    return command;
} */

void free_command(char** command) {
    int i;
    for (i = 0; command[i] != NULL; i++){
        /* printf("- parse_command - %p, '%s'\n", command[i], command[i]); */
        free(command[i]);
        command[i] = NULL;
    }

    /* printf("- command - %p\n", command); */
    free(command);
    command = NULL;
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