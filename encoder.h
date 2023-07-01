#ifndef ENCODER_H
#define ENCODER_H

#include "constants.h"
#include "errors.h"
#include "parser.h"
#include "linkedlist.h"

extern const char* opcodes[];
extern const char* registers[];
extern const char* data_types[];

/**
 * Checks if a token is present in the given array of strings.
 *
 * @param token The token to search for.
 * @param arr   The array of strings to search in.
 * @return      1 if the token is found, 0 otherwise.
 */
int is_in(char *token, const char **arr);

/**
 * Checks if a string represents a number with the expected type.
 *
 * @param operand  The string to check.
 * @param expected The expected type of the number (e.g., 'int', 'float').
 * @return         1 if the string represents a number of the expected type, 0 otherwise.
 */
int is_num(char *operand, int expected);

/**
 * Gets the size of an instruction line.
 *
 * @param line The instruction line.
 * @return     The size of the line.
 */
int get_line_size(char **line);

/**
 * Gets the index of an opcode in the opcode array.
 *
 * @param opcode The opcode to search for.
 * @return       The index of the opcode, or -1 if not found.
 */
int get_opcode_index(char *opcode);

/**
 * Gets the index of a register in the register array.
 *
 * @param reg The register to search for.
 * @return    The index of the register, or -1 if not found.
 */
int get_reg_index(char *reg);

/**
 * Converts a string representation of a number to an integer.
 *
 * @param data The string representation of the number.
 * @return     The converted integer value.
 */
int to_int(char *data);

/**
 * Gets the size of an instruction in memory.
 *
 * @param line The instruction line.
 * @return     The size of the instruction.
 */
int sizeof_instruction(char **line);

/**
 * Validates the syntax of a label.
 *
 * @param token The label to validate.
 * @return      1 if the label syntax is valid, 0 otherwise.
 */
int validate_label_syntax(char *token);

/**
 * Gets the type of a token.
 *
 * @param token The token to determine the type of.
 * @return      The type of the token.
 */
int get_token_type(char *token);

/**
 * Gets the size of the word queue.
 *
 * @param word The word queue.
 * @return     The size of the queue.
 */
int _get_queue_size(machine_w*** word);

/**
 * Frees the memory allocated for the word queue.
 *
 * @param word  The word queue.
 * @param size  The size of the queue.
 */
void free_word_queue(machine_w*** word, int size);

/**
 * Allocates memory for the word queue.
 *
 * @param word  The word queue.
 * @param size  The size of the queue.
 */
void allocate_word_queue(machine_w*** word, int size);

/**
 * Encodes a register word.
 *
 * @param reg1 The first register.
 * @param reg2 The second register.
 * @return     The encoded machine word.
 */
machine_w* encode_reg_w(char *reg1, char *reg2);

/**
 * Encodes an immediate or direct word.
 *
 * @param operand     The operand to encode.
 * @param placeholder The placeholder value to use.
 * @return            The encoded machine word.
 */
machine_w* encode_imdt_drct_w(char* operand, int placeholder);

/**
 * Encodes the first word of an instruction.
 *
 * @param line         The instruction line.
 * @param label        The label of the instruction.
 * @param opcode_index The index of the opcode.
 * @param start_index  The start index of the instruction.
 * @return             An array of encoded machine words.
 */
machine_w** encode_first_w(char** line, char* label, int opcode_index, int start_index);

/**
 * Encodes the data words of an instruction.
 *
 * @param line         The instruction line.
 * @param label        The label of the instruction.
 * @param token_type   The type of the token.
 * @param start_index  The start index of the instruction.
 * @return             An array of encoded machine words.
 */
machine_w** encode_data_w(char **line, char *label, int token_type, int start_index);

machine_w** encode(char** instruction);

#endif /* ENCODER_H */

