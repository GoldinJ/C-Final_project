#include "macro.h"

void expand_macros(FILE *fsrc, char* filename) {
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

    if (fdst == NULL) {
        fprintf(stderr, "Failed to create %s.am file\n", filename);
        return;
    }

    while ((line = get_line(fsrc)) != NULL) {

        if (line[0] == '\0') {
            free(line);
            continue;
        }

        /* Clean any leading/trailing whitespace */
        clean_whitespaces(line);
        line_copy = duplicateString(line);
        instruction = parse_command(line_copy);

        if (is_macro == FALSE) {
            /* Check if the current line defines a new macro */
            if (strcmp(instruction[0], "mcro") == 0) {
                is_macro = TRUE;
                mcro.name = duplicateString(instruction[1]);
                mcro.body = NULL;
                mcro.line_cnt = 0;
                insert(macro_table, mcro.name, mcro.body);

            } else if ((tmp = get(macro_table, instruction[0])) != 0) {
                /* If the line matches an existing macro, expand it */
                for (i = 0; tmp[i] != NULL; i++) {
                    fprintf(fdst, "%s\n", tmp[i]);
                }
                tmp = NULL;

            } else {
                /* Otherwise, write the line as is to the output */
                fprintf(fdst, "%s\n", line);
            }

        } else {
            /* Inside a macro definition */
            if (strcmp(instruction[0], "endmcro") == 0) {
                /* End of macro, store its definition */
                is_macro = FALSE;
                appendString(&(mcro.body), &(mcro.line_cnt), NULL);
                insert(macro_table, mcro.name, mcro.body);
                free(mcro.name);
            } else {
                /* Append lines to the current macro definition */
                appendString(&(mcro.body), &(mcro.line_cnt), line);
            }
        }
        
        free(line);
        free(line_copy);
        free_command(instruction);
    }

    /* Free memory and close files */
    freeHashtableStrings(macro_table);
    fclose(fdst);
}

int checkout_macros(char* filename) {
    FILE *fsrc = NULL;

    /* Attempt to open the source file for reading */
    fsrc = open_file(filename, ".as", "r");

    if (fsrc == NULL) {
        return FALSE;
    }

    /* Expand macros in the source file and clean up */
    expand_macros(fsrc, filename);
    fclose(fsrc);

    return TRUE;
}
