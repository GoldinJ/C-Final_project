#ifndef UTILITIES_H
#define UTILITIES_H

#include "constants.h"


char* int_to_Base64(int binary);
void print_last_12_bits(unsigned int num);
char* duplicateString(const char* source);
void appendString(char*** list, int* size, const char* str);
FILE *open_file(char* filename, char* extension, char* mode);
void remove_file(char *filename, char *extension);
#endif 

