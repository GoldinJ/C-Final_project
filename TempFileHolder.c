


char** Order_array(char* line, int* numRows) {
    const int MAX_ROWS = 100;  // Maximum number of rows allowed

    char** orderArray = malloc(MAX_ROWS * sizeof(char*));
    int i = 0;  // Rows


    // Tokenize the line by whitespace characters ' ' and '\t'
    char* token = strtok(line, " \t");
    while (token != NULL && i < MAX_ROWS) {
        orderArray[i] = malloc((strlen(token) + 1) * sizeof(char));
        strcpy(orderArray[i], token);
        i++;
        token = strtok(NULL, " \t");
    }

    *numRows = i;

    return orderArray;
}




/*Function for finding the name of the macro and storing it an array and saving the macro body into a 2D array corresponding to the name */

MacroData FindMacroData(char** command) {
    /*INSTRUCTION: MacroData macro_data = FindMacroData(command); */

    int i=0 ;
    int count = 0;
    char** macro_name=NULL;
    char*** macro_body=NULL;
    int* macro_body_counts=NULL;
    char** current_macro_body = NULL;
    MacroData macro_data;

    while(orderArray[i]!=NULL) {
        if (strcmp(orderArray[i], "mcro") == 0) {
            if (orderArray[i + 1]!=NULL) {
                i+=2; /*move the i to point at the macro body*/

                /*allocate memory for the macro names*/
                macro_name=realloc(macro_name,(count+1) * sizeof (char*));
                if(macro_name==NULL) {
                    fprintf(stderr,"\"macro_name: Memory allocation failed\"\n");
                    exit(1);
                }
                macro_name[count]= (char*)malloc(strlen(orderArray[i - 1]) + 1);
                if (macro_name[count] == NULL) {
                    fprintf(stderr, "\"macro_name: Memory allocation failed\"\n");
                    exit(1);
                }

                /*allocate memory for the macro bodies*/
                macro_body_counts = realloc(macro_body_counts,(count+1)*sizeof(int));
                if(macro_body_counts==NULL) {
                    fprintf(stderr,"\"macro_body_counts: Memory allocation failed\"\n");
                    exit(1);
                }
                macro_body_counts[count]=0;

                /*processing macro_body */
                while(orderArray[i]!=NULL && strcmp(orderArray[i],"endmcro")!=0){
                    /* Allocate memory for current_macro_body*/
                    current_macro_body = realloc(current_macro_body, (macro_body_counts[count] + 1) * sizeof(char*));
                    if(current_macro_body==NULL) {
                        fprintf(stderr,"\"current_macro_body: Memory allocation failed\"\n");
                        exit(1);
                    }
                    current_macro_body[macro_body_counts[count]]= (char*)malloc(strlen(orderArray[i]) + 1);
                    if (current_macro_body[macro_body_counts[count]] == NULL) {
                        fprintf(stderr, "\"current_macro_body: Memory allocation failed\"\n");
                        exit(1);
                    }
                    strcpy(current_macro_body[macro_body_counts[count]], orderArray[i]);
                    i++;
                    macro_body_counts[count]++;
                }
                /*allocate memory for macro body*/
                macro_body= realloc(macro_body,(count+1)*sizeof (char**));
                if(macro_body==NULL) {
                    fprintf(stderr,"\"macro_body: Memory allocation failed\"\n");
                    exit(1);
                }
                macro_body[count]=current_macro_body;
                count++;
            }
        }
        i++;
    }


    macro_name = realloc(macro_name,(count+1)*sizeof(char*));
    if(macro_name==NULL) {
        fprintf(stderr,"\"macro_name: Memory allocation failed\"\n");
        exit(1);
    }
    macro_name[count]=NULL;

    /*Create MacroData structure to hold the name , body , count the lines*/
    macro_data.macro_names = macro_name;
    macro_data.macro_bodies = macro_body;
    macro_data.macro_body_counts = macro_body_counts;

    /*free the memory allocated to orderArray array */
    free_orderArray(orderArray);
    return macro_data;
}

void MacroLayout(char** orderArray, MacroData macro_data) {
    int i = 0;

    while (orderArray[i] != NULL) {
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
                    orderArray[i++] = macro_data.macro_bodies[macro_index][k];
                    k++;
                }
            } else {
                i += 2;
            }
        } else {
            i++;
        }
    }
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

void free_orderArray(char** orderArray) {
    int i;
    for (i = 0; orderArray[i] != NULL; i++) {
        free(orderArray[i]);
    }
    free(orderArray);
}

