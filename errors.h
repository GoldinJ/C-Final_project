#define SET_TO_RED(msg) "\033[1;31m" msg "\033[0m"
/*                           ==================================== Syntax Errors ====================================                                */
#define ILLEGAL_CHARACTER_IN_LABEL SET_TO_RED("Error: ") "A label cannot begin with - '%c'\n"
#define TOO_FEW_OPERANDS      SET_TO_RED("Error: ") "too few operands for - '%s' command\n"
#define TOO_MANY_OPERANDS     SET_TO_RED("Error: ") "too many operands for - '%s' command\n"
#define INVALID_OPERAND_TYPE  SET_TO_RED("Error: ") "Invalid operand type for '%s' command. Operand: %d\n"
#define INVALID_LABEL_SYNTAX  SET_TO_RED("Error: ") "Invalid character - '%c' in label defenition\n"
#define INVALID_LABEL_LENGTH  SET_TO_RED("Error: ") "Invalid label length - '%s'\n"