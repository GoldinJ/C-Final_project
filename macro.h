#ifndef MACRO_H
#define MACRO_H

#include "constants.h"
#include "error.h"
#include "parser.h"
#include "utilities.h"
#include "hashtable.h"

void expand_macros(FILE *fsrc, char* filename);
void checkout_macros(char* filename);

#endif