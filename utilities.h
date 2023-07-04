#ifndef UTILITIES_H
#define UTILITIES_H

#include "constants.h"


char* int_to_Base64(int binary);
void print_last_12_bits(unsigned int num);
void append(char** array, const char* newString);
void freeArray(char** array);
void printArray(char **array);

#endif 

