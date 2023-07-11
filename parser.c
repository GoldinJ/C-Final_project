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


/*Finds the name of each macro and body of each macro and stores them in MacroData struct */
MacroData NameBodyExtractor(FILE* fptr) {
    MacroData macro_data;
    int macro_count = 0;
    int line_count = 0;
    char* line;
    int body_count;
    char** macro_body;
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
            char* name_start = line + 4;
            while (*name_start != '\0' && isspace(*name_start)) {
                name_start++;
            }
            strcpy(macro_name, name_start);

            /* Initialize macro body */
            body_count = 0;
            macro_body = malloc(10 * sizeof(char*));

            if (macro_name == NULL || macro_body == NULL) {
                printf("Order_array: Memory allocation failed");
                exit(1);
            }

            /* Read macro body until "endmcro" is encountered */
            while ((line = get_line(fptr)) != NULL && strcmp(line, "endmcro") != 0) {
                macro_body[body_count] = malloc((strlen(line) + 1) * sizeof(char));
                strcpy(macro_body[body_count], line);
                body_count++;

                /* Resize macro_body array if necessary */
                if (body_count % 10 == 0) {
                    macro_body = realloc(macro_body, (body_count + 10) * sizeof(char*));
                    if (macro_body == NULL) {
                        printf("Order_array: Memory allocation failed");
                        exit(1);
                    }
                }
            }

            macro_body[body_count] = NULL;  /* Add NULL terminator to the macro body array */

            /* Store macro name, body, and count in MacroData */
            macro_names[macro_count] = macro_name;
            macro_bodies[macro_count] = macro_body;
            macro_body_counts[macro_count] = body_count;

            macro_count++;

            /* Resize arrays if necessary */
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

    macro_names[macro_count] = NULL;     /* Add NULL terminator to the macro name array */
    macro_bodies[macro_count] = NULL;    /* Add NULL terminator to the macro body array */
    macro_body_counts[macro_count] = -1; /* Set the count of the last macro to -1       */

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




/*Erase the lines that start with "mcro" or "endmcro" from the file*/
void line_eraser(const char* filename) {

    int i;
    FILE* output_file;
    /* Create a 2D array to store file content*/
    char file_content[MAX_LINE_LEN][MAX_LINE_LEN];
    int line_count = 0;
    char* line;


    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        printf("Failed to open file: %s\n", filename);
        return;
    }



    while ((line = get_line(input_file)) != NULL) {
        if (strncmp(line, "mcro", 4) == 0 || strcmp(line, "endmcro") == 0) {
            /* Skip lines starting with "mcro" and "endmcro" */
            free(line);
            continue;
        }
        strncpy(file_content[line_count], line, MAX_LINE_LEN - 1);
        file_content[line_count][MAX_LINE_LEN - 1] = '\0';
        line_count++;
        free(line);
        if (line_count >= MAX_LINE_LEN) {
            printf("Maximum line count exceeded.\n");
            break;
        }
    }

    fclose(input_file);

    /* Rewrite the file with the modified content*/
    output_file = fopen(filename, "w");
    if (output_file == NULL) {
        printf("Failed to open file for rewriting: %s\n", filename);
        return;
    }

    for ( i = 0; i < line_count; i++) {
        fputs(file_content[i], output_file);
        fputc('\n', output_file);
    }

    fclose(output_file);

    printf("Lines starting with 'mcro' and 'endmcro' have been deleted from the file: %s\n", filename);
}


void macro_layout(MacroData macro_data, const char* filename) {
    int i;
    FILE* temp_file;
    FILE* output_file;
    char line[MAX_LINE_LEN];
    char* trimmed_line;
    int line_contains_macro;
    int name_length;
    int position_index;
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        printf("Failed to open temporary file.\n");
        fclose(file);
        return;
    }


    while (fgets(line, sizeof(line), file) != NULL) {
        trimmed_line = line;
        strip(trimmed_line);

        line_contains_macro = 0;

        for (i = 0; macro_data.macro_names[i] != NULL; i++) {
            char* macro_name = macro_data.macro_names[i];
            char* macro_body = macro_data.macro_bodies[i][0];

            /* Replace macro name if found within the line */
            char* position = strstr(trimmed_line, macro_name);
            if (position != NULL) {
                line_contains_macro = 1;
                name_length = strlen(macro_name);
                position_index = position - trimmed_line;

                /* Write the line until the position of the macro name */
                trimmed_line[position_index] = '\0';

                /* Trim leading whitespace characters, including tabs */
                while (*trimmed_line != '\0' && isspace(*trimmed_line))
                    trimmed_line++;

                /* Write the trimmed line to the temporary file */
                fputs(trimmed_line, temp_file);
                fputc('\n', temp_file);

                /* Write the macro body */
                fputs(macro_body, temp_file);
                fputc('\n', temp_file);

                /* Move the position to the end of the macro name */
                position += name_length;

                // Write the rest of the line after the macro name
                fputs(position, temp_file);
                break;
            }
        }

        /* Write the line as it is if no macro name found */
        if (!line_contains_macro) {
            // Trim leading whitespace characters, including tabs
            while (*trimmed_line != '\0' && isspace(*trimmed_line))
                trimmed_line++;

            /* Write the trimmed line to the temporary file */
            fputs(trimmed_line, temp_file);
            fputc('\n', temp_file);
        }
    }

    fclose(file);
    fclose(temp_file);

    /* Rewrite the original file with the content of the temporary file */
    output_file = fopen(filename, "w");
    if (output_file == NULL) {
        printf("Failed to open file for rewriting: %s\n", filename);
        return;
    }

    temp_file = fopen("temp.txt", "r");
    if (temp_file == NULL) {
        printf("Failed to open temporary file.\n");
        fclose(output_file);
        return;
    }

    while (fgets(line, sizeof(line), temp_file) != NULL) {
        fputs(line, output_file);
    }

    fclose(output_file);
    fclose(temp_file);

    /* Clear the content of the temporary file for future use */
    temp_file = fopen("temp.txt", "w");
    fclose(temp_file);

    printf("Macro layout has been applied to the file: %s\n", filename);
}



/*
int main() {
    const char* filename = "x.txt";

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        return 1;
    }

    MacroData macro_data = NameBodyExtractor(file);
    fclose(file);

    line_eraser(filename);

    macro_layout(macro_data, filename);

    freeNameAndBody(macro_data);

    return 0;
}
*/


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