#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LABELS 100
#define MAX_LABEL_LENGTH 1024
#define MAX_LABEL_LENGTH1 32


#define SET_TO_RED(msg) "\033[1;31m" msg "\033[0m"
/*                           ==================================== Syntax Errors ====================================                                */
#define ILLEGAL_CHARACTER_IN_LABEL SET_TO_RED("Error: ") "A label cannot contain - '%c'\n"
#define TOO_FEW_OPERANDS     SET_TO_RED("Error: ") "too few operands \n"
#define TOO_MANY_OPERANDS     SET_TO_RED("Error: ") "extra operand - '%s' found \n"
#define INVALID_OPERAND_TYPE  SET_TO_RED("Error: ") "Invalid operand '%s' \n"
#define INVALID_LABEL_SYNTAX  SET_TO_RED("Error: ") "Invalid character - '%c' in label defenition\n"
#define INVALID_LABEL_LENGTH  SET_TO_RED("Error: ") "Invalid label length - '%s'\n"
#define INVALID_STRING_DEFINITION_ERROR (SET_TO_RED("Error: ") "String definition is invalid %s ")
#define EMPTY_DATA_FIELD    (SET_TO_RED("Error: ") "data field is NULL ")
#define INVALID_DATA_CHAR    (SET_TO_RED("Error: ") "invalid character- %c ")
#define INVALID_COMMA    (SET_TO_RED("Error: ") "invalid comma after the last number ")
#define INVALID_SEMICOLON    (SET_TO_RED("Error: ") "Invalid ; placement %c ")
#define MISSING_LABEL   (SET_TO_RED("Error: ") "Label is missing ")
#define INVALID_LABEL   (SET_TO_RED("Error: ") "Invalid Label %s ")
#define DUP_LABEL SET_TO_RED("Error: ") "Duplicate label '%s'\n"
#define INVALID_OPCODES      SET_TO_RED("Error: ") "Invalid opcode - '%s' command\n"
#define DUPLICATE_LABEL SET_TO_RED("Error: ") "Duplicate label found in '%s'\n"
#define LABEL_IS_RESERVED_WORD SET_TO_RED("Error: ") "Label is a reserved word - '%s'\n"



/*                           ==================================== File Errors ====================================            */

#define FILE_NOT_FOUND SET_TO_RED("Error: ") "File not found - '%s'\n"




/*Function to check if a given string is a valid label and doesnt exceed 31 characters*/  /*DONE*/


int is_valid_label( char *line) {
    char *label_end;
    int label_len;
    int i;
    char ch;

    /* Check if line is empty */
    if (line == NULL || *line == '\0') {
        return 0;
    }

    /* Find the first occurrence of ':' */
    label_end = strchr(line, ':');
    if (label_end == NULL) {
        /* No label found, so it's not an error */
        return 1;
    }

    /* Calculate the length of the label (excluding the colon ':') */
    label_len = label_end - line;

    /* Check label length */
    if (label_len >= MAX_LABEL_LENGTH1) {
        printf(INVALID_LABEL_LENGTH,line);
        return 0;
    }

    /* Check label characters */
    for (i = 0; i < label_len; i++) {
        ch = line[i];
        if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'))) {
            printf(ILLEGAL_CHARACTER_IN_LABEL, ch);
            return 0;
        }
    }

    return 1;
}


/* Array to hold labels found in the program */
static char labels[MAX_LABELS][MAX_LABELS];
static int num_labels = 0;


/* Function to check if a given line might contain duplicate labels*/                     /*DONE*/
int is_single_label(char *line) {
    int label_len;
    int i;
    char *colon_pos;
    char label[MAX_LABEL_LENGTH];

    /* Check if the line contains ':' (colon) */
    colon_pos = strchr(line, ':');
    if (colon_pos == NULL) {
        return 1; /* No ':' found, no label in the line, return 1 */
    }

    /* Store the label (token before the ':' in the line) */
    label_len = colon_pos - line;
    strncpy(label, line, label_len);
    label[label_len] = '\0';

    /* Check if the stored label is a valid label */
    if (is_valid_label(label)) {
        /* Check if the label already exists in the array */
        for (i = 0; i < num_labels; i++) {
            if (strcmp(label, labels[i]) == 0) {
                printf(DUPLICATE_LABEL, label);
                /* Found a duplicate label, print an error and return 0 */
                return 0;
            }
        }

        /* Store the label in the array for future use */
        strncpy(labels[num_labels], label, MAX_LABEL_LENGTH);
        num_labels++;

        return 1; /* Valid label found and stored */
    } else {
        printf(INVALID_LABEL_SYNTAX, line[label_len]);
        return 1; /* Invalid label, assume the line is correct and return 1 */
    }
}



/*Function to hold all the saved word*/
int is_reserved_word(char *word) {
    int i;
    int num_reserved_words;

    char *reserved_words[] = {
            "@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7",
            "mov", "cmp", "add", "sub", "lea", "not", "clr", "inc",
            "dec", "jmp", "bne", "red", "jsr", "prn", "rts", "stop",
            ".data", ".string", ".entry", ".extern"
    };

    num_reserved_words = sizeof(reserved_words) / sizeof(reserved_words[0]);

    for (i = 0; i < num_reserved_words; i++) {
        if (strcmp(word, reserved_words[i]) == 0) {
            return 1; /* It is a reserved word*/
        }
    }

    return 0; /* It is not a reserved word*/
}



/*Function to check if the label is not a saved word*/                                    /*DONE*/
/* Function to check if the label is not a saved word */
int label_is_not_a_saved_word(char *line) {
    int label_len;
    char *colon_pos;
    char label[MAX_LABEL_LENGTH];

    /* Check if the line contains ':' (colon) */
    colon_pos = strchr(line, ':');
    if (colon_pos == NULL) {
        return 1; /* No ':' found, no label in the line, return 1 */
    }

    /* Calculate the length of the label (excluding the colon ':') */
    label_len = colon_pos - line;

    /* Check if the label length is more than 10 characters */
    if (label_len > 10) {
        return 1; /* Return 1 if the label length is > 10 */
    }

    /* Copy the label (without colon) from the input line */
    strncpy(label, line, label_len);
    label[label_len] = '\0'; /* Null-terminate the label*/

    /* Check if the stored label is not a reserved word */
    if (!is_reserved_word(label)) {
        return 1; /* Not a reserved word, return 1 */
    } else {
        printf(LABEL_IS_RESERVED_WORD,label);
        return 0; /* Reserved word found, return 0 */
    }
}





/*Function to help is_valid_operand_amount */
int is_valid_label_check(char *operand) {
    int i;
    char ch;
    int has_letter;
    int has_digit;

    /* Check if operand is empty */
    if (operand == NULL || *operand == '\0') {
        return 0;
    }

    /* Check label characters */
    has_letter = 0;
    has_digit = 0;

    for (i = 0; operand[i] != '\0'; i++) {
        ch = operand[i];
        if (isalpha(ch)) {
            has_letter = 1;
        } else if (isdigit(ch)) {
            has_digit = 1;
        } else {
            return 0; /* Invalid character in the label */
        }
    }

    /* Check if the label has at least one letter and can have digits */
    if (has_letter && (has_letter || has_digit)) {
        /* Check if the label is not a reserved word */
        if (!is_reserved_word(operand)) {
            return 1; /* Valid label */
        }
    }

    return 0; /* Invalid label or a reserved word */
}


/*Function to help is_valid_operand_amount*/
int is_valid_register_check(char *word) {

    int i;


    char *valid_registers[] = {"@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7"};
    int num_registers = sizeof(valid_registers) / sizeof(valid_registers[0]);

    for (i = 0; i < num_registers; i++) {
        if (strcmp(word, valid_registers[i]) == 0) {
            return 1; /* Valid register*/
        }
    }

    return 0; /* Not a valid register*/
}

/*Function to help is_valid_operand_amount*/
int is_valid_integer(char *word) {
    int i;

    /* Check if the word is empty*/
    if (word == NULL || *word == '\0') {
        return 0;
    }

    /* Check if the first character is a sign (+/-)*/
    i = 0;
    if (word[0] == '+' || word[0] == '-') {
        i = 1; /* Skip the sign character if present*/
    }

    /* Check if the remaining characters are digits*/
    for (; word[i] != '\0'; i++) {
        if (!isdigit(word[i])) {
            return 0; /* Not a valid integer, contains non-digit characters*/
        }
    }

    /* Check if the word is not just a sign (e.g., "+" or "-")*/
    if (i == 1 && (word[0] == '+' || word[0] == '-')) {
        return 0;
    }

    return 1; /* Valid integer*/
}

/*Function to help is_valid_operand_amount*/
int is_valid_opcode_check(char *word) {
    int i;

    char *valid_opcodes[] = {"mov", "cmp", "add", "sub", "lea", "not", "clr", "inc",
                             "dec", "jmp", "bne", "red", "jsr", "prn", "rts", "stop"};
    int num_opcodes = sizeof(valid_opcodes) / sizeof(valid_opcodes[0]);

    for (i = 0; i < num_opcodes; i++) {
        if (strcmp(word, valid_opcodes[i]) == 0) {
            return 1; /* Valid opcode*/
        }
    }

    return 0; /* Not a valid opcode*/
}


/* Helper function to check if a character is a delimiter */
int is_delimiter(char c) {
    return c == ',' || c == ' ' || c == '\t' || c == '\n';
}

/* Helper function to extract a token from the input string */
void extract_token(char *input, char *token, int *start, int *end) {

    int i = *start;
    int j;
    while (is_delimiter(input[i])) {
        i++;
    }
    *start = i;

    j = i;
    while (input[j] != '\0' && !is_delimiter(input[j])) {
        j++;
    }
    *end = j;

    memcpy(token, &input[*start], *end - *start);
    token[*end - *start] = '\0';
}


/*Function that checks if the opcode received the right operands */
int is_valid_operand_amount(char *line) {
    char delimiters[] = ", \t\n"; /* Separators: comma, space, and tab */
    char *token;
    int num_operands;
    char operand1[MAX_LABEL_LENGTH];
    char operand2[MAX_LABEL_LENGTH];

    /* Check if the line is empty or contains only delimiters */
    if (line == NULL || *line == '\0' || strspn(line, delimiters) == strlen(line)) {
        return 0;
    }

    /* Check if the line starts with ".extern" or ".entry" */
    if (strncmp(line, ".extern", 7) == 0 || strncmp(line, ".entry", 6) == 0) {
        return 1; /* Ignore the whole line if it starts with .extern or .entry */
    }

    /* Check if the line contains ".data" or ".string" */
    if (strstr(line, ".data") != NULL || strstr(line, ".string") != NULL) {
        return 1; /* Ignore the line if it contains .data or .string */
    }

    /* Tokenize the line and look for the opcode */
    token = strtok(line, delimiters);
    while (token != NULL && !is_valid_opcode_check(token)) {
        token = strtok(NULL, delimiters);
    }

    if (token == NULL) {
        printf(INVALID_OPCODES,token);
        return 0; /* Opcode not found or line contains only delimiters */
    }

    /* Based on the opcode groups, check the corresponding operand fields */
    if (strcmp(token, "mov") == 0 || strcmp(token, "add") == 0 || strcmp(token, "sub") == 0) {
        /* Group 1: Check the first and second fields for valid (REGISTER or INTEGER or LABEL) */
        num_operands = 0;

        while ((token = strtok(NULL, delimiters)) != NULL) {
            num_operands++;
            if (num_operands > 2) {
                printf(TOO_MANY_OPERANDS, token);
                return 0; /* Too many operands for mov, add, sub */
            }
            /* Check for valid operand based on the order (first or second) */
            if (num_operands == 1) {
                if (!is_valid_register_check(token) && !is_valid_integer(token) && !is_valid_label_check(token)) {
                    printf(INVALID_OPERAND_TYPE,token);
                    return 0; /* Invalid first operand */
                }
                strcpy(operand1, token);
            } else if (num_operands == 2) {
                if (!is_valid_register_check(token) && !is_valid_label_check(token)) {
                    printf(INVALID_OPERAND_TYPE,token);
                    return 0; /* Invalid second operand */
                }
                strcpy(operand2, token);
            }
        }
        if (num_operands < 2) {
            printf(TOO_FEW_OPERANDS);
            return 0; /* Too few operands for mov, add, sub */
        }
        return num_operands >= 2 ; /* At least two operands required for mov, add, sub  */
    } else if (strcmp(token, "cmp") == 0) {
        /* Group 2: Check the first and second fields for valid (LABEL or REGISTER or INTEGER) */
        num_operands = 0;


        while ((token = strtok(NULL, delimiters)) != NULL) {
            num_operands++;
            if (num_operands > 2) {
                printf(TOO_MANY_OPERANDS, token);
                return 0; /*Too many operands for cmp*/
            }
            if (!is_valid_register_check(token) && !is_valid_integer(token) && !is_valid_label_check(token)) {
                printf(INVALID_OPERAND_TYPE,token);
                return 0; /* Invalid operand*/
            }
            if (num_operands == 1) {
                strcpy(operand1, token);
            } else if (num_operands == 2) {
                strcpy(operand2, token);
            }
        }
        return num_operands >= 2 ;
    } else if (strcmp(token, "not") == 0 || strcmp(token, "clr") == 0 || strcmp(token, "inc") == 0 ||
               strcmp(token, "dec") == 0 || strcmp(token, "jmp") == 0 || strcmp(token, "bne") == 0 ||
               strcmp(token, "red") == 0 || strcmp(token, "jsr") == 0) {
        /* Group 3: Check the second field for valid (LABEL or REGISTER or INTEGER) */
        token = strtok(NULL, delimiters); /* Get the second operand field */
        if (token == NULL) {

            return 0; /* Missing second operand */
        }
        if (!is_valid_register_check(token) && !is_valid_label_check(token)) {
            printf(INVALID_OPERAND_TYPE,token);
            return 0; /* Invalid operand */
        }

        token = strtok(NULL, delimiters); /* Get the third operand field */
        return token == NULL; /* No third operand should be present */
    } else if (strcmp(token, "lea") == 0) {
        /* Group 4: Check the first field for a valid LABEL and the second field for a valid REGISTER */
        num_operands = 0;

        while ((token = strtok(NULL, delimiters)) != NULL) {
            num_operands++;
            if (num_operands > 2) {
                printf(TOO_MANY_OPERANDS, token);
                return 0; /* Too many operands for lea */
            }
            /* Check for valid operand based on the order (first or second) */
            if (num_operands == 1) {
                if (!is_valid_label_check(token)) {
                    printf(INVALID_OPERAND_TYPE,token);
                    return 0; /* Invalid first operand (should be LABEL) */
                }
                strcpy(operand1, token);
            } else if (num_operands == 2) {
                if (!is_valid_register_check(token) && !is_valid_label_check(token)) {
                    printf(INVALID_OPERAND_TYPE,token);
                    return 0; /* Invalid second operand (should be LABEL or REGISTER) */
                }
                strcpy(operand2, token);
            }
        }
        if (num_operands < 2) {
            printf(TOO_FEW_OPERANDS);
            return 0; /* Too few operands for mov, add, sub */
        }
        return num_operands >= 2 ; /* At least two operands required for mov, add, sub  */
    } else if (strcmp(token, "prn") == 0) {
        /* Group 5: Check the first field for empty and the second field for valid (LABEL or REGISTER or INTEGER) */
        token = strtok(NULL, delimiters); /* Get the second operand field */
        if (token == NULL) {
            return 0; /* Missing second operand */
        }
        if (!is_valid_register_check(token) && !is_valid_integer(token) && !is_valid_label_check(token)) {
            printf(INVALID_OPERAND_TYPE,token);
            return 0; /* Invalid operand */
        }

        token = strtok(NULL, delimiters); /* Get the third operand field */
        return token == NULL; /* No third operand should be present */
    } else if (strcmp(token, "rts") == 0 || strcmp(token, "stop") == 0) {
        /* Groups 6: These opcodes have no operands, so they are valid */
        token = strtok(NULL, delimiters); /* Check if there's an unexpected second operand */
        return token == NULL; /* No second operand should be present */
    } else {
        /* Invalid opcode */
        return 0;
    }
}


/*Function that helps is_directive to validate .data content*/
int is_valid_data(const char *str) {
    char *endptr;

    size_t len = strlen(str);
    if (len == 0) {
        printf(EMPTY_DATA_FIELD,str);
        return 0; /* Empty data*/
    }

    while (*str) {
        /* Attempt to convert the current part of the string to a long integer */
        strtol(str, &endptr, 10);

        /* Check if strtol failed or if there is an invalid character after the number */
        if (str == endptr || (*endptr && *endptr != ',' && !isspace(*endptr))) {
            printf(INVALID_DATA_CHAR,str);
            return 0; /* Invalid number or invalid character*/
        }

        str = endptr;
        if (*str == ',') {
            str++; /* Move past the comma */
        } else if (*str != '\0') {
            return 0; /* Invalid character after the number */
        }
    }

    /* Check if the last character processed is a comma */
    if (*(str - 1) == ',') {
        printf(INVALID_COMMA,str);
        return 0; /* Comma after the last number is not allowed */
    }

    return 1; /* Valid data */
}

/*Function for validating .string .data and the content*/                                          /*DONE*/
int is_directive(char* line) {
    char* colon_pos;

    /* Skip any leading whitespaces*/
    while (*line != '\0' && isspace(*line)) {
        line++;
    }

    /* Check if the line starts with a label (ending with a colon ':')*/
    colon_pos = strchr(line, ':');
    if (colon_pos != NULL) {
        line = colon_pos + 1; /* Move the pointer after the colon*/
    }

    /* Skip any leading whitespaces after the label*/
    while (*line != '\0' && isspace(*line)) {
        line++;
    }

    /* Check if the line contains ".string" or ".data"*/
    if (strstr(line, ".string") == NULL && strstr(line, ".data") == NULL) {
        return 1; /* Neither .string nor .data found, ignore the line*/
    }

    /* Check if the line starts with ".string" or ".data"*/
    if (strncmp(line, ".string", 7) == 0) {
        line += 7; /* Move the pointer after ".string"*/

        /* Skip whitespaces after ".string"*/
        while (*line != '\0' && isspace(*line)) {
            line++;
        }

        /* Check if the next character is a double quote*/
        if (*line == '\"') {
            line++; /* Move the pointer after the double quote*/

            /* Check if the content inside the double quotes consists of letters only*/
            while (*line != '\0' && *line != '\"') {
                if (!isalpha(*line)) {
                    printf(INVALID_STRING_DEFINITION_ERROR,line);
                    return 0; /* Invalid character inside the string*/
                }
                line++;
            }

            /* Check if the closing double quote is found*/
            if (*line == '\"') {
                line++; /* Move the pointer after the closing double quote*/

                /* Skip any whitespaces after the string*/
                while (*line != '\0' && isspace(*line)) {
                    line++;
                }

                /* Check if the rest of the line is empty (no other characters after the string)*/
                if (*line == '\0') {
                    return 1; /* Valid .string directive*/
                }
            }
        }
    } else if (strncmp(line, ".data", 5) == 0) {
        line += 5; /* Move the pointer after ".data"*/

        /* Skip whitespaces after ".data"*/
        while (*line != '\0' && isspace(*line)) {
            line++;
        }

        /* Check if the next characters are valid data*/
        if (is_valid_data(line)) {
            return 1; /* Valid .data directive*/
        }
    }

    /* If the line does not match any valid directive, it is invalid*/
    return 0;
}


/*Function that checks for semicolon displacement*/                                                /*DONE*/
int semicolon_displacement(char *line) {
    /* Skip leading whitespaces*/
    while (*line != '\0' && isspace(*line)) {
        line++;
    }

    /* Check if the line starts with a semicolon ';'*/
    if (*line == ';') {
        return 1; /* Ignore the whole line*/
    }

    /* Search for semicolon ';' anywhere in the line*/
    while (*line != '\0') {
        if (*line == ';') {
            printf(INVALID_SEMICOLON,line);
            return 0; /* Semicolon found anywhere but the beginning of the line*/
        }
        line++;
    }

    return 1; /* No semicolon found in the line*/
}



/*Array to hold labels found in .entry , .extern*/
char all_labels[MAX_LABELS][MAX_LABELS];
int num_all_labels = 0;

/*Function that finds .entry , .extern validate the label and store the label in an array*/       /*DONE*/
int valid_entry_extern(char *line) {
    char *token;
    int i;

    /* Check if the line is empty */
    if (line == NULL || *line == '\0') {
        return 0;
    }

    /* Tokenize the line using spaces and tabs as delimiters */
    token = strtok(line, " \t");
    if (token == NULL) {
        return 0; /* Empty line, nothing to check */
    }

    /* Check if the first token is ".entry" or ".extern" */
    if (strcmp(token, ".entry") == 0 || strcmp(token, ".extern") == 0) {
        /* Move to the next token */
        token = strtok(NULL, " \t");

        /* Check if there is a second token */
        if (token == NULL) {
            printf(MISSING_LABEL,line);
            return 0; /* Missing label after ".entry" or ".extern" */
        }

        /* Check if the second token is a valid label */
        if (!is_valid_label_check(token)) {
            printf(INVALID_LABEL,token);
            return 0; /* Invalid label after ".entry" or ".extern" */
        }

        /* Check if the label already exists in the array */
        for (i = 0; i < num_all_labels; i++) {
            if (strcmp(all_labels[i], token) == 0) {
                printf(DUP_LABEL,token);
                return 0; /* Label already encountered */
            }
        }

        /* Add the label to the array and increment the counter */
        strcpy(all_labels[num_all_labels], token);
        num_all_labels++;

        return 1; /* Valid .entry or .extern with a valid label */
    }

    return 1; /* No .entry or .extern found at the beginning of the line */
}


/*Mother Function that gathers all of the function above to check the line*/
int error_check(char *line, char *filename, int line_num) {

    int result = 1; /* Initialize the result to 1 (assume success) */

    /* Check for semicolon displacement */
    if (!semicolon_displacement(line)) {
        result = 0;
    }

    /*Check for duplicate labels*/
    if (!is_valid_label(line)) {
        result = 0;
    }

    /* Check if the line might contain duplicate labels */
    if (!is_single_label(line)) {
        result = 0;
    }

    /* Check if the line contains a valid directive */
    if (!is_directive(line)) {
        result = 0;
    }

    /* Check if the label is not a saved word */
    if (!label_is_not_a_saved_word(line)) {
        result = 0;
    }

    /* Check if the operand amount is valid */
    if (!is_valid_operand_amount(line)) {
        result = 0;
    }

    /* Check if the .entry or .extern with a valid label */
    if (!valid_entry_extern(line)) {
        result = 0;
    }

    return result;
}







int main() {
    char lines[][100] = {
            "MAIN: mov @r3, LENGTH",                  /* Valid label */
            "MAINDIDJIJDIWJDIWJIDJWIJDIdwdwdwdwdwdWJDIDJWIDJIWD: mov @r3, @r4",  /* Valid label */
            "TOOLONG_LABEL_NAME: mov @r3, @r4",       /* Invalid - Label name too long */
            "illegal_label!: add @r3, @r4",           /* Invalid - Illegal characters in label */
            ".xtern MAIN",                         /* Invalid - Empty label name */
            ".entry MAIN",
            "ANOTHER_LABEL: sub @r1, @r2",            /* Valid label */
            "MAINDIDJIJDIWJDIWJIDJWIJDIdwdwdwdwdwdWJDIDJWIDJIW: mov @r3, @r4",   /* Invalid - Label name too long */
            "mylabel: add @r3, @r4"     ,              /* Valid label */
            "mylabel: add @r3, @r4",
            "MAIN: add @r3, @r4",
            "mov: add @r3, @r4"


    };

    char *filename = "TestFile.txt";
    char line[] = "at line number:";
    char v[] = "Valid line";

    int i;
    for (i = 0; i < sizeof(lines) / sizeof(lines[0]); i++) {
        int valid = error_check(lines[i], filename, i );
        if (valid)
            printf("%d %s\n\n", i , v);
        else
            printf("%s %s %d\n\n", filename, line, i );
    }

    return 0;


}
/*
int main() {
    char filename[] = "your_file_name.txt";
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    int line_num = 1;
    int valid = 1; // Assume success

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        // Remove the newline character at the end of the line
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        // Call error_check function for each line
        if (!error_check(line, filename, line_num)) {
            valid = 0; // Error detected
        }

        line_num++;
    }

    fclose(file);

    if (valid) {
        printf("All lines are valid!\n");
    } else {
        printf("Errors detected in the file.\n");
    }

    return 0;
}


*/
