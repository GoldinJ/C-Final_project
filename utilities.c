#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define LINE_LEN 50
#define MAX_LINE_LEN 100
#define COMMAND_LEN 4
#define PASS 1
#define FAILED 0

char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

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
    int line_len = LINE_LEN;
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

        else if(cnt == line_len){
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

    /* Validate the number of commas */
    if (commaCount == 0) {
        return 0;  /* No commas found */
    }

    return 1;
}

char** parse_command(char* line){
    int i=0;
    char **command = malloc(COMMAND_LEN * sizeof(char *));

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

char* int_to_Base64(int binary) {
    char* base64 = malloc(sizeof(char) * 3);
    base64[0] = base64_table[(binary >> 6) & 0x3F];
    base64[1] = base64_table[binary & 0x3F];
    base64[2] = '\0';
    return base64;
}

int main() {
    /* int binary_number = 2572;
    char* base64 = int_to_Base64(binary_number);
    printf("Base64 representation: %s\n", base64);
    free(base64); */
    char line1[] = ".data 7, -57 , +17, 9";
    char line2[] = ".data 7, -57 , +17, , 9";
    char line3[] = ".data 7, -57 , +17, 9,";
    char line4[] = "   .data 7, -57 , +17, 9   ";
    char line5[] = ".data 7, -57 , +17,, 9";
    int i = 0;
    int j = 0;
    char **cmnd = NULL;
    char *arr[] = {line1, line2, line3, line4, line5, NULL};
    
    for(i = 0; arr[i] != NULL; i++ ){
        if (validate_syntax(arr[i])){
            printf("%s\n", arr[i]);
            j = 0;
            cmnd = parse_command(arr[i]);
            while (cmnd[j] != NULL){
                printf("%s\n", cmnd[j++]);
            }
            printf("========================\n");
        }
    }

    return 0;

}