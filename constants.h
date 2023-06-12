
#define MAX_LABEL_LEN 31
#define MAX_INT_VALUE 1023
#define MIN_INT_VALUE -1023

char* opcodes[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
enum Opcodes_enum {MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP};

char* registers[] = {"@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7"};
enum Registers_enum {R0, R1, R2, R3, R4, R5, R6, R8};

char* data_types[] = {".entry", ".extern", ".data", ".string"};
enum dt_enum {ENTRY, EXTERN, DATA, STR};

enum addressing {IMDT=1, DRCT=3, DRCT_REG=5};
enum are {A, R, E};

/*Defines a 12 bit machine word - the fisrt word of instruction*/
typedef struct first_w
{
    int ARE:2;
    int dest_operand:3;
    int opcode:4;
    int src_operand:3;
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
typedef struct data_w{
    int data: 12
}data_w;








