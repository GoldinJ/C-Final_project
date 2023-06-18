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
#define IS_CHAR(c) ((IN_RANGE(c, 'A', 'Z') || IN_RANGE(c, 'a', 'z')))
#define IS_LABEL(str) (*((str) + strlen(str) - 1) == ':')
#define START_WITH(token, sign) (*token == sign)
#define END_WITH(token, sign) (*((token) + strlen(token) - 1) == sign)

enum booleans {FALSE, TRUE};

enum Opcodes_enum {MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP};

enum Registers_enum {R0, R1, R2, R3, R4, R5, R6, R8};

enum dt_enum {ENTRY, EXTERN, DATA, STR};

enum addressing {IMDT=1, DRCT=3, DRCT_REG=5};
enum are {A, R, E};

enum TokenType{
    TOKEN_LABEL,
    TOKEN_OPCODE,
    TOKEN_DATATYPE,
    TOKEN_REGISTER,
    TOKEN_INEGER,
    TOKEN_VAR,
    TOKEN_STRING

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
    int ARE:2;
    int src_operand:10;

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
    int placeholder;
    union word
    {
        first_w *f_word;
        imdt_drct_w *im_drct_w;
        reg_w *r_word;
        data_w *d_word;

    }word;
    
}machine_w;

typedef struct operand_node
{
    int type;
    char *token;

}operand_node;

typedef struct data_node
{
    int type;
    char **data;
}data_node;

typedef struct string_node
{
    int type;
    char *token;

}string_node;


typedef struct instruction_2op
{
    /* Defines an instruction with 2 operands (mov, add, cmp, etc..)*/
    char* label;
    int opcode_index;
    operand_node first;
    operand_node second;

}instruction_2op;

typedef struct instruction_1op
{
    /* Defines an instruction with one operand (jmp, bne, not, etc..)*/
    char* label;
    int opcode_index;
    operand_node first;
}instruction_1op;

typedef struct instruction_0
{
    /* Defines an instruction with no operands (stop, rts)*/
    int opcode_index;
}instruction_0;



typedef struct instruction_w
{
    char *label;
    union instruction
    {
        instruction_2op i2;
        instruction_1op i1;
        instruction_0 i0;
        
    }instruct;
    

}instructtion_w;


#endif









