#ifndef MACRO_H
#define MACRO_H

#include "constants.h"
#include "error.h"
#include "parser.h"
#include "utilities.h"
#include "hashtable.h"

typedef struct Macro{
    char *name;
    char **body;
    int line_cnt;

}Macro;

void expand_macros(FILE *fsrc, char* filename);
int checkout_macros(char* filename);

#endif