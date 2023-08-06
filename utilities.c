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

FILE *open_file(char* filename, char* extension, char* mode){
    FILE *fptr;
    char* _filename = (char*)malloc(strlen(filename)+strlen(extension)+1);

    if(_filename == NULL){
        fprintf(stderr, "open_file: Memory allocation failed\n");
        exit(1);                     
    }

    strcpy(_filename, filename);
    strcat(_filename, extension);

    fptr = fopen(_filename, mode);

    if(fptr == NULL){
        fprintf(stderr, "open_file: Failed to open - '%s'\n", _filename);
        return NULL;
    }

    free(_filename);

    return fptr;       
}

void remove_file(char *filename, char *extension){
    char* _filename = (char*)malloc(strlen(filename)+strlen(extension)+1);

    if(_filename == NULL){
        fprintf(stderr, "open_file: Memory allocation failed\n");
        exit(1);                     
    }

    strcpy(_filename, filename);
    strcat(_filename, extension);

    remove(_filename);
    free(_filename);
}


char* duplicateString(const char* source) {
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

void appendString(char*** list, int* size, const char* str) {
    char** newList = (char**)realloc(*list, (*size + 1) * sizeof(char*));
    if (newList == NULL) {
        fprintf(stderr, "appendString: Memory allocation failed!\n");
        return;
    }

    newList[*size] = duplicateString(str);
    *list = newList;
    (*size) += 1;
}