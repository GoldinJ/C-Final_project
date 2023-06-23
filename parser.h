#ifndef PARSER_H
#define PARSER_H

void strip(char *str);
char* get_line();
int validate_syntax(char* line);
char** parse_command(char* line);
void free_command(char** command);
MacroData FindMacroData(char** command);
void FreeMacroData(MacroData macroData);
#endif