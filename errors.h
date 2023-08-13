#ifndef ERRORS_H
#define ERRORS_H

#include "constants.h"

#define MAX_LABELS 100
#define MAX_LABEL_LENGTH 1024
#define MAX_LABEL_LENGTH1 32
#define MAX_LINE_LENGTH 80

#define SET_TO_RED(msg) "\033[1;31m" msg "\033[0m"
#define ERROR_LOCATION "(In file: %s.am, line :%d - '%s')\n\n"
#define ERROR_LOCATION_NL "(In file: %s.am, line: %d)\n\n"
/*                           ==================================== Syntax Errors ====================================                                */

#define INVALID_LABEL_SYNTAX  SET_TO_RED("Error: ") "Invalid character - '%c' in label defenition\n"
#define INVALID_LABEL_LENGTH  SET_TO_RED("Error: ") "Invalid label length - '%s'\n"
#define INVALID_REGISTER      SET_TO_RED("Error: ") "Invalid register - '%s'\n"
#define NOT_AN_INTEGER        SET_TO_RED("Error: ") "Is not an integer - '%s'\n"
#define INVALID_SIGN          SET_TO_RED("Error: ") "Invalid sign for this operand type - '%c'\n"
#define INVALID_INTEGER_SYNTAX SET_TO_RED("Error: ") "Invalid integer syntax - '%s'\n"
#define IVALID_INTEGER_VALUE_TOO_BIG SET_TO_RED("Error: ") "Integer is too big - '%d', max allowed value is 1023\n"
#define IVALID_INTEGER_VALUE_TOO_SMALL SET_TO_RED("Error: ") "Integer is too small - '%d', min allowed value is -1023\n"
#define WRONG_OPERAND_TYPE SET_TO_RED("Error: ") "Wrong operand type ('%s') - must be a label\n"
#define ILLEGAL_CHARACTER_IN_LABEL SET_TO_RED("Error: ") "A label cannot contain - '%c'\n"
#define TOO_FEW_OPERANDS     SET_TO_RED("Error: ") "too few operands \n"
#define TOO_MANY_OPERANDS     SET_TO_RED("Error: ") "extra operand - '%s'  \n"
#define INVALID_OPERAND_TYPE  SET_TO_RED("Error: ") "Invalid operand - '%s' \n"
#define INVALID_LABEL_SYNTAX  SET_TO_RED("Error: ") "Invalid character - '%c' in label defenition\n"
#define INVALID_LABEL_LENGTH  SET_TO_RED("Error: ") "Invalid label length - '%s'\n"
#define INVALID_STRING_DEFINITION_ERROR (SET_TO_RED("Error: ") "String definition is invalid - %s \n")
#define EMPTY_DATA_FIELD    (SET_TO_RED("Error: ") "data field is NULL \n")
#define INVALID_DATA_CHAR    (SET_TO_RED("Error: ") "Invalid character- %c \n")
#define INVALID_COMMA    (SET_TO_RED("Error: ") "invalid comma - '%c' \n")
#define INVALID_SEMICOLON    (SET_TO_RED("Error: ") "Invalid ; placement - %c \n")
#define MISSING_LABEL   (SET_TO_RED("Error: ") "Label is missing in - %s \n")
#define INVALID_LABEL   (SET_TO_RED("Error: ") "Invalid Label - %s \n")
#define DUP_LABEL SET_TO_RED("Error: ") "Duplicate label - '%s'\n"
#define INVALID_OPCODES      SET_TO_RED("Error: ") "Invalid opcode - '%s' \n"
#define DUPLICATE_LABEL SET_TO_RED("Error: ") "Duplicate label found in - '%s'\n"
#define LABEL_IS_RESERVED_WORD SET_TO_RED("Error: ") "Label is a reserved word - '%s'\n"
#define LABEL_DECLARATION_MISSING SET_TO_RED("Error: ") "Label - '%s' - declaration is missing\n"
#define ERRORS_DETECTED SET_TO_RED("Error: ") "Errors have been detected in the file \n"
#define EXTRA_CHARACTERS_AFTER_STRING     SET_TO_RED("Error: ") "character definition not allowed here- '%s' \n"
#define MISSING_CLOSING_QUOTE     SET_TO_RED("Error: ") "missing '\"' in %s \n"
#define NO_VALID_DATA  SET_TO_RED("Error: ") "Invalid data input  \n"
#define INVALID_OPCODE_COMMA  SET_TO_RED("Error: ") "Invalid comma found in line \n"
#define EXTRA_CHARACTERS_AFTER_LABEL SET_TO_RED("Error: ") "Invalid definition after the label - %s\n"
#define INVALID_DATA_RANGE    SET_TO_RED("Error: ") "Illegal number %c \n"

/*                           ==================================== File Errors ====================================            */
                
#define FILE_NOT_FOUND SET_TO_RED("Error: ") "File not found - '%s'\n"

  /*                           ==================================== Memory Errors ====================================            */

#define MEMORY_ALLOCATION_FAILED(msg) SET_TO_RED("Error: ") msg ": Memory allocation failed\n"
#define MEMORY_OVERFLOW SET_TO_RED("Error: ") "Memory overflow\n"

int error_check(char *line, char *filename, int line_num);

#endif