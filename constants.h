#ifndef CONSTANTS_H
#define CONSTANTS_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define INSTR_SIZE 20
#define MAX_LINE_LEN 81
#define MAX_LABEL_LEN 31
#define MAX_INT_VALUE 1023
#define MIN_INT_VALUE -1023

#define IN_RANGE(c, from, to) (c>=from && c<=to)
#define IS_VALID_CHAR(c) (isalnum(c) || (c) == '+' || (c) == '-' || (c) == '.' || (c) == '@' || (c) == '"')
#define IS_LABEL(str) (*((str) + strlen(str) - 1) == ':')
#define START_WITH(token, sign) (*token == sign)
#define END_WITH(token, sign) (*((token) + strlen(token) - 1) == sign)

enum booleans {FALSE, TRUE};

enum Opcodes_enum {MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP};

enum Registers_enum {R0, R1, R2, R3, R4, R5, R6, R8};

enum dt_enum {ENTRY, EXTERN, DATA, STR};

enum addressing {IMDT=1, DRCT=3, DRCT_REG=5};
enum are {A, E, R};

enum token_type{
    TOKEN_LABEL_DEFENITION,
    TOKEN_LABEL,
    TOKEN_OPCODE,
    TOKEN_ENTRY,
    TOKEN_EXTERN,
    TOKEN_DATA,
    TOKEN_REGISTER,
    TOKEN_INEGER,
    TOKEN_VAR,
    TOKEN_STRING,
    TOKEN_UNDEFINED = -1

};

enum node_type {
    NODE_ENTRY, 
    NODE_EXTERN, 
    NODE_FIRST_W, 
    NODE_IMDT_DRCT_W, 
    NODE_REG_W,
    NODE_DATA_W,
    NODE_STRING_W
};


/*Defines a 12 bit machine word - the fisrt word of instruction*/
typedef struct first_w
{
    unsigned int ARE:2;
    unsigned int dest_operand:3;
    unsigned int opcode:4;
    unsigned int src_operand:3;
    
}first_w;

/*Defines a 12 bit an additional machine word for the operand with immediate or direct addressing*/
typedef struct imdt_drct_w
{
    unsigned int ARE:2;
    unsigned int src_operand:10;

}imdt_drct_w;


/*Defines a 12 bit an additional machine word where at least one of the operands is a registrer (@r0 - @r7)*/
typedef struct reg_w
{
    int ARE:2;
    int dest_operand:5;
    int src_operand:5;

}reg_w;


/*Defines a 12 bit machine word containing data from .data or .string*/
typedef struct data_w
{
    int data: 12;

}data_w;

/*Defines a 12 bit general machine word: first word of instruction or an additional word of following types: immediate/direct additional word, register word or data word */
/*Contains a label if exists*/

typedef struct machine_w
{
    char *label;
    int node_type;
    int placeholder;
    union word
    {
        first_w *f_word;
        imdt_drct_w *im_drct_w;
        reg_w *r_word;
        data_w *d_word;

    }word;
    
}machine_w;

/*struct to hold the name , body , number of lines*/
typedef struct {
    char** macro_names;
    char*** macro_bodies;
    int* macro_body_counts;
} MacroData;


typedef struct Instruction{
    char *label;
    int opcode_index;
    int num_operands;
    int operand1_type;
    int operand2_type;
}Instruction;

typedef struct Directive{
    char *label;
    int type;
    int num_operands;
    int operand_type;

}Directive;

#endif









