#ifndef PARSER_H
#define PARSER_H

#include "constants.h"
#include "errors.h"

void strip(char *str);
char* get_line();
int validate_syntax(char* line);
char** parse_command(char* line);
void free_command(char** command);

#endif