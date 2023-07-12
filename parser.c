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
    char** macro_names = malloc(10 * sizeof(char*));
    char*** macro_bodies = malloc(10 * sizeof(char**));
    int* macro_body_counts = malloc(10 * sizeof(int));
    char* macro_name;
    char** macro_body;
    char* name_start;
    int body_count;
    if (macro_names == NULL || macro_bodies == NULL || macro_body_counts == NULL) {
        printf("Order_array: Memory allocation failed");
        exit(1);
    }

    while ((line = get_line(fptr)) != NULL) {
        if (strncmp(line, "mcro", 4) == 0) {
            /* Extract macro name */
            macro_name = malloc((strlen(line) - 4 + 1) * sizeof(char));
            name_start = line + 4;
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


void line_eraser(const char* filename) {
    char* line;
    int inside_macro = 0;
    FILE* output_file;
    FILE* temp_file;

    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    output_file = fopen("temp.txt", "w");
    if (output_file == NULL) {
        printf("Failed to open temporary file.\n");
        fclose(input_file);
        return;
    }



    while ((line = get_line(input_file)) != NULL) {
        if (strncmp(line, "mcro", 4) == 0) {
            /* Skip lines starting with "mcro" and set inside_macro flag to true */
            inside_macro = 1;
            free(line);
            continue;
        } else if (strcmp(line, "endmcro") == 0) {
            /* Skip lines starting with "endmcro" and reset inside_macro flag to false */
            inside_macro = 0;
            free(line);
            continue;
        }

        if (!inside_macro) {
            /* Write non-macro lines to the temporary file */
            fputs(line, output_file);
            fputc('\n', output_file);
        }

        free(line);
    }

    fclose(input_file);
    fclose(output_file);

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

    while ((line = get_line(temp_file)) != NULL) {
        fputs(line, output_file);
        fputc('\n', output_file);
        free(line);
    }

    fclose(output_file);
    fclose(temp_file);

    /* Clear the content of the temporary file for future use */
    temp_file = fopen("temp.txt", "w");
    fclose(temp_file);

    printf("Lines starting with 'mcro' and 'endmcro' have been deleted from the file: %s\n", filename);
}


void macro_layout(MacroData macro_data, const char* filename) {
    int i, j;
    FILE* temp_file;
    char line[MAX_LINE_LEN];
    char* trimmed_line;
    int line_contains_macro;
    int name_length;
    int position_index;
    char* macro_name;
    char** macro_body;
    char* position;
    char* body_line;
    char new_filename[MAX_LINE_LEN];


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
            macro_name = macro_data.macro_names[i];
            macro_body = macro_data.macro_bodies[i];

            /* Replace macro name if found within the line */
            position = strstr(trimmed_line, macro_name);
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
                for (j = 0; macro_body[j] != NULL; j++) {
                    /* Trim leading whitespace characters, including tabs */
                    body_line = macro_body[j];
                    while (*body_line != '\0' && isspace(*body_line))
                        body_line++;

                    /* Write the trimmed line to the temporary file */
                    fputs(body_line, temp_file);
                    fputc('\n', temp_file);
                }

                /* Move the position to the end of the macro name */
                position += name_length;

                /* Write the rest of the line after the macro name*/
                fputs(position, temp_file);
                break;
            }
        }

        /* Write the line as it is if no macro name found */
        if (!line_contains_macro) {
            /* Trim leading whitespace characters, including tabs*/
            while (*trimmed_line != '\0' && isspace(*trimmed_line))
                trimmed_line++;

            /* Write the trimmed line to the temporary file */
            fputs(trimmed_line, temp_file);
            fputc('\n', temp_file);
        }
    }

    fclose(file);
    fclose(temp_file);

    /* Create a new filename with .am extension */
    strcpy(new_filename, filename);
    strcat(new_filename, ".am");

    /* Rename the temporary file to the new filename */
    if (rename("temp.txt", new_filename) != 0) {
        printf("Failed to rename the file.\n");
        return;
    }

    printf("Macro layout has been applied. New file created: %s\n", new_filename);
}
void File_rename(const char* filename) {
    char* new_filename;
    char* dot;
    FILE* original_file;
    FILE* new_file;
    int ch;
    size_t length;

    /* Find the position of the first dot in the original file name */
    dot = strchr(filename, '.');
    if (dot == NULL) {
        printf("Invalid file name.\n");
        return;
    }

    /* Calculate the length of the portion before the dot */
    length = dot - filename;

    /* Allocate memory for the new file name */
    new_filename = malloc((length + 4) * sizeof(char));  /* Additional 4 characters for ".as" and null terminator */
    if (new_filename == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    /* Copy the characters before the dot to the new file name */
    strncpy(new_filename, filename, length);
    new_filename[length] = '\0';

    /* Concatenate ".as" to the new file name */
    strcat(new_filename, ".am");

    /* Open the original file for reading */
    original_file = fopen(filename, "r");
    if (original_file == NULL) {
        printf("Failed to open the original file.\n");
        free(new_filename);
        return;
    }

    /* Open the new file for writing */
    new_file = fopen(new_filename, "w");
    if (new_file == NULL) {
        printf("Failed to create the new file.\n");
        fclose(original_file);
        free(new_filename);
        return;
    }

    /* Copy the content from the original file to the new file */
    while ((ch = fgetc(original_file)) != EOF) {
        fputc(ch, new_file);
    }

    /* Close the files */
    fclose(original_file);
    fclose(new_file);

    printf("File renamed: %s\n", new_filename);

    free(new_filename);
}


/*
int main() {
    const char* original_file = "x.as";
    const char* renamed_file = "x.am";
    FILE* file;
    MacroData macro_data;

    File_rename(original_file);

    file = fopen(renamed_file, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    macro_data = NameBodyExtractor(file);

    fclose(file);

    macro_layout(macro_data, renamed_file);

    line_eraser(renamed_file);

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