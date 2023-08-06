#define SET_TO_RED(msg) "\033[1;31m" msg "\033[0m"
#define ERROR_LOCATION "(at:%s.am:line:%d)\n"
/*                           ==================================== Syntax Errors ====================================                                */
#define UNEXPECTED_COMMA SET_TO_RED("Error: ") "Comma in an unexpected position - %s\n"
#define ILLEGAL_CHARACTER_IN_LABEL SET_TO_RED("Error: ") "A label cannot begin with - '%c'\n"
#define TOO_FEW_OPERANDS      SET_TO_RED("Error: ") "too few operands for - '%s' instruction\n"
#define TOO_MANY_OPERANDS     SET_TO_RED("Error: ") "too many operands for - '%s' instruction\n"
#define INVALID_OPERAND_TYPE  SET_TO_RED("Error: ") "Invalid operand type for '%s' command. Operand: %d\n"
#define INVALID_LABEL_SYNTAX  SET_TO_RED("Error: ") "Invalid character - '%c' in label defenition\n"
#define INVALID_LABEL_LENGTH  SET_TO_RED("Error: ") "Invalid label length - '%s'\n"
#define INVALID_REGISTER      SET_TO_RED("Error: ") "Invalid register - '%s'\n"
#define NOT_AN_INTEGER        SET_TO_RED("Error: ") "Is not an integer - '%s'\n"
#define INVALID_SIGN          SET_TO_RED("Error: ") "Invalid sign for this operand type - '%c'\n"
#define INVALID_INTEGER_SYNTAX SET_TO_RED("Error: ") "Invalid integer syntax - '%s'\n"
#define IVALID_INTEGER_VALUE_TOO_BIG SET_TO_RED("Error: ") "Integer is too big - '%d', max allowed value is 1023\n"
#define IVALID_INTEGER_VALUE_TOO_SMALL SET_TO_RED("Error: ") "Integer is too small - '%d', min allowed value is -1023\n"
#define WRONG_OPERAND_TYPE SET_TO_RED("Error: ") "Wrong operand type ('%s') - must be a label\n"
                            
  /*                           ==================================== File Errors ====================================            */
                
#define FILE_NOT_FOUND SET_TO_RED("Error: ") "File not found - '%s'\n"

  /*                           ==================================== Memory Errors ====================================            */

#define MEMORY_ALLOCATION_FAILED(msg) SET_TO_RED("Error: ") msg ": Memory allocation failed\n"