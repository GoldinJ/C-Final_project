#include "utilities.h"


char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* int_to_Base64(int binary) {
    char* base64 = malloc(sizeof(char) * 3);
    base64[0] = base64_table[(binary >> 6) & 0x3F];
    base64[1] = base64_table[binary & 0x3F];
    base64[2] = '\0';
    return base64;
}

void print_last_12_bits(unsigned int num) {
    int mask = (1 << 12) - 1;  
    int last12Bits = num & mask;
    int i;
    int bit;

    for (i = 11; i >= 0; i--) {
        bit = (last12Bits >> i) & 1;
        printf("%d", bit);
    }
    printf("\n");
}

void append(char** array, const char* newString) {
    int size = 0;

    /* Calculate the current size of the array */
    if (array != NULL) {
        while (array[size] != NULL) {
            size++;
        }
    }
    
    size++;

    array = (char**)realloc(array, (size + 1) * sizeof(char*));

    array[size - 1] = (char*)malloc((strlen(newString) + 1) * sizeof(char));

    strcpy(array[size - 1], newString);

    array[size] = NULL;
}

void freeArray(char** array){
    int i;
    for (i = 0; array[i] != NULL; i++) {
        free(array[i]);
    }
    free(array);
}

void printArray(char **array){
    int i = 0;

    while(array[i] != NULL){
        printf("%s |", array[i]);
        i++;
    }
    printf("\n");
}