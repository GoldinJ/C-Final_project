


char* opcodes[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
enum Opcodes_enum {MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP};

char* registers[] = {"@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7"};
enum Registers_enum {R0, R1, R2, R3, R4, R5, R6, R8};

char* data_types[] = {".entry", ".extern", ".data", ".string"};
enum dt_enum {ENTRY, EXTERN, DATA, STR}; 



