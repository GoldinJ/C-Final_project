#include "constants.h"
#include "errors.h"
#include "parser.h"
#include "linkedlist.h"

const char* opcodes[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", NULL};
const char* registers[] = {"@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7", NULL};
const char* data_types[] = {".entry", ".extern", ".data", ".string", NULL};

int is_in(char *token, const char **arr){
    int i = 0;

    while(arr[i] != NULL){
        if (strcmp(token, arr[i]) == 0)
            return TRUE;

        i++;
    }


    return FALSE;
}

int is_num(char *operand){

    int op_len = strlen(operand);
    int i = 0;

    if(operand[i] == '+' || operand[i] == '-' || IN_RANGE(operand[i], '0', '9'))
        i++;
    else
        return FALSE;

    for (; i<op_len; i++){
        if (!IN_RANGE(operand[i], '0', '9'))
            return FALSE;
    }


    return TRUE;
}

int get_line_size(char **line){
    int i = 0;
    int cnt = 0;

    while(line[i++] != NULL){
        cnt++;
    }

    return cnt;
}

int get_opcode_index(char *opcode){

    int i = 0;

    while(opcodes[i] != NULL){
        if (strcmp(opcodes[i], opcode) == 0)
            return i;
        i++;
    }

    return -1;
}

int get_reg_index(char *reg){
    int i = 0;

    while(registers[i] != NULL){
        if (strcmp(registers[i], reg) == 0)
            return i;
        i++;
    }

    return TOKEN_UNDEFINED;

}

int to_int(char *data){

    int sign = 1;  /* Default sign is positive */
    int result = 0;
    int i = 0;

    if (data == NULL || *data == '\0') {
        return MAX_INT_VALUE+1;
    }
    
    if (data[0] == '+' || data[0] == '-') {
        if (data[0] == '-') {
            sign = -1;  /* Set sign to negative */
        }
        i = 1;  /* Start from the next character */
    }
    
    while (data[i] != '\0') {
        
        if (IN_RANGE(data[i], '0', '9')) {
            result = result * 10 + (data[i] - '0');
            i++;
        } else {
            return MAX_INT_VALUE + 1;
        }
    }

    return sign * result;
}

int get_token_type(char *token){

    if(IS_LABEL(token))
        return TOKEN_LABEL;

    else if(is_in(token, opcodes))
        return TOKEN_OPCODE;

    else if(is_in(token, registers))
        return TOKEN_REGISTER;

    else if(is_in(token, data_types))
        return TOKEN_DATATYPE;

    else if(is_num(token))
        return TOKEN_INEGER;

    else if (START_WITH(token, '"') && END_WITH(token, '"'))
        return TOKEN_STRING;

    else
        return TOKEN_VAR;

    return TOKEN_UNDEFINED;
}

machine_w* encode_reg_w(char *reg1, char *reg2){

    /*<label>: <opcode> <reg1>, <reg2>*/

    machine_w* m_word = (machine_w*)malloc(sizeof(machine_w));
    reg_w *register_w = (reg_w*)malloc(sizeof(reg_w));

    register_w->ARE = A;

    if(reg1 != NULL && reg2 != NULL){
        register_w->src_operand = get_reg_index(reg1);
        register_w->dest_operand = get_reg_index(reg2);
    }
    else if(reg1 == NULL){
        register_w->src_operand = 0;
        register_w->dest_operand = get_reg_index(reg2);
    }
    else{
        register_w->src_operand = get_reg_index(reg1);
        register_w->dest_operand = 0;
    }

    
    m_word->label = NULL;
    m_word->node_type = NODE_REG_W;
    m_word->word.r_word = register_w;

    return m_word;

}

machine_w* encode_imdt_drct_w(char* operand, int placeholder){
    int token_type = get_token_type(operand);
    machine_w* m_word = (machine_w*)malloc(sizeof(machine_w));
    imdt_drct_w* im_dr_word = (imdt_drct_w*)malloc(sizeof(im_dr_word));

    m_word->label = NULL;
    m_word->node_type = NODE_IMDT_DRCT_W;

    if (token_type == TOKEN_INEGER){
        im_dr_word -> ARE = A;
        im_dr_word -> src_operand = to_int(operand);
        m_word ->word.im_drct_w = im_dr_word;
    }
    else if (token_type == TOKEN_VAR)
    {
        m_word->placeholder = placeholder;
        im_dr_word->ARE=0;
        im_dr_word->src_operand = 0;
        m_word->word.im_drct_w = im_dr_word;
    }
    else{
        free(m_word);
        free(im_dr_word);
        return NULL;
    }

    return m_word;

}

machine_w** encode_first_w(char** line){

   
    int i = 0;
    int tmp_i;
    int opcode_index = -1;
    int arr_size = 4;
    machine_w** m_word = (machine_w**)malloc(arr_size*sizeof(machine_w*));
    first_w* f_word = (first_w*)malloc(sizeof(first_w));

    for (tmp_i = 0; tmp_i<arr_size; tmp_i++)
        m_word[tmp_i] =(machine_w*)malloc(sizeof(machine_w));


    
    m_word[0]->node_type = NODE_FIRST_W;

    if (get_token_type(line[0]) == TOKEN_LABEL){
        m_word[0]->label = line[0];
        i++;
    }

    if (get_token_type(line[i]) == TOKEN_OPCODE){
        opcode_index = get_opcode_index(line[i]);
        i++;
    }
    
    else{
        for (tmp_i = 0; tmp_i < arr_size; tmp_i++)
            free(m_word[tmp_i]);
        free(m_word);
        free(m_word);
        free(f_word);
        return NULL;
    }
    
    switch (opcode_index)
        {
            case CMP:
            case MOV:
            case ADD:
            case SUB:
            case LEA:

                if(line[i] == NULL || line[i+1] ==NULL){
                    printf(TOO_FEW_OPERANDS, opcodes[opcode_index]);
                    free(m_word);
                    free(f_word);
                    return NULL;
                }

                f_word->ARE = A;
                f_word->opcode = opcode_index;

                if(is_num(line[i])){
                    f_word->src_operand = IMDT;
                    m_word[1] = encode_imdt_drct_w(line[i], FALSE);
                    i++;
                }
                else if(get_reg_index(line[i]) != TOKEN_UNDEFINED && get_reg_index(line[i+1]) != TOKEN_UNDEFINED){
                    f_word ->src_operand = DRCT_REG;
                    f_word ->dest_operand = DRCT_REG;
                    m_word[0]->word.f_word = f_word;
                    m_word[1] = encode_reg_w(line[i], line[i+1]);
                    free(m_word[2]);
                    free(m_word[3]);
                    m_word[2] = NULL;
                    m_word[3] = NULL;
                    break;
                }
                else if (get_reg_index(line[i]) != TOKEN_UNDEFINED)
                {
                    f_word->src_operand = DRCT_REG;
                    m_word[1] = encode_reg_w(line[i], NULL);
                    i++;
                }
                else{
                    f_word->src_operand = DRCT;
                    m_word[1] = encode_imdt_drct_w(line[i], TRUE);
                    i++;
                }

               
                if(is_num(line[i]) && opcode_index != CMP){
                    printf(INVALID_OPERAND_TYPE, opcodes[opcode_index], i);
                }
                else if(is_num(line[i]) && opcode_index == CMP){
                    f_word->dest_operand = IMDT;
                    m_word[2] = encode_imdt_drct_w(line[i], FALSE);
                }
                else if (get_reg_index(line[i]) != TOKEN_UNDEFINED)
                {
                    f_word->dest_operand = DRCT_REG;
                    m_word[2] = encode_imdt_drct_w(line[i], FALSE);
                }
                else
                    f_word->dest_operand = DRCT;
                    m_word[2] = encode_imdt_drct_w(line[i], TRUE);
               
                m_word[0]->word.f_word = f_word;
                free(m_word[3]);
                m_word[3] = NULL;

                break;

            case NOT:
            case CLR:
            case INC:
            case DEC:
            case JMP:
            case BNE:
            case RED:
            case PRN:
            case JSR:
                f_word->ARE = A;
                f_word->opcode = opcode_index;

                if(line[i] == NULL){
                    printf(TOO_FEW_OPERANDS, opcodes[opcode_index]);
                }

                else if(is_num(line[i])){
                    f_word->dest_operand = IMDT;
                    m_word[1] = encode_imdt_drct_w(line[i], FALSE);
                }
                else if (get_reg_index(line[i]) != TOKEN_UNDEFINED)
                {
                    f_word->dest_operand= DRCT_REG;
                    m_word[1] = encode_reg_w(line[i], NULL);
                }
                else
                    f_word->dest_operand = DRCT;
                    m_word[1] = encode_imdt_drct_w(line[i], TRUE);
                
                m_word[0]->word.f_word = f_word;
                free(m_word[2]);
                free(m_word[3]);
                m_word[2] = NULL;
                m_word[3] = NULL;
                break;

            case RTS:
            case STOP:
                if(line[i] != NULL){
                    printf(TOO_MANY_OPERANDS, opcodes[opcode_index]);
                    break;
                }


                f_word->ARE = A;
                f_word->src_operand = 0;
                f_word->opcode = opcode_index;
                f_word->dest_operand = 0;
                m_word[0]->word.f_word = f_word;
                free(m_word[1]);
                free(m_word[2]);
                free(m_word[3]);
                m_word[1] = NULL;
                m_word[2] = NULL;
                m_word[3] = NULL;
                break;
            
            default:
                printf("Error: Undefined opcode - '%s'\n", line[i]);
                break;
        }

    return m_word;
}

machine_w* encode_data_w(char *token, int token_type){

    machine_w* m_word = (machine_w*)malloc(sizeof(machine_w));
    data_w *d_word = (data_w*)malloc(sizeof(data_w));
    int num;

    if (token_type == TOKEN_UNDEFINED)
    {
        printf("Error in encode_data_w");
        free(m_word);
        free(d_word);
        return NULL;
    }
    

    if(token_type == TOKEN_INEGER){
        num = to_int(token);

        if (IN_RANGE(num, MIN_INT_VALUE, MAX_INT_VALUE)){
            d_word->data = num;
            m_word->node_type = NODE_DATA_W;
            m_word->word.d_word = d_word;
            return m_word;
        }
        else{
            printf("Error: Token - '%s' is out of range\n", token);
            free(m_word);
            free(d_word);
            return NULL;
        }

    }
    else if (token_type == TOKEN_STRING){
        
        return NULL;

    }
    
    return NULL;
}

machine_w* encode(LinkedList *list, char** instruction){
    

    return NULL;

}

int main (){

    char *line;
    char *line_copy;
    char **instruction = NULL;
    int line_len;
    int i = 0;
    machine_w* tmp;
    machine_w** words;

    LinkedList list;
    list.head = NULL;
    list.tail = NULL;

    while ((line = get_line()) != NULL) {
        line_len = strlen(line);
        line_copy = malloc((line_len + 1) * sizeof(char));
        strcpy(line_copy, line);

        if (!validate_syntax(line_copy)) {
            free(line_copy);
            free(line);
            continue;
        }

        instruction = parse_command(line_copy);
        words = encode_first_w(instruction);
        i = 0;
        while(words[i] != NULL){
            tmp = words[i];
            if (i == 0)
                add_node(&list, instruction, tmp);
            else
                add_node(&list, NULL, tmp);
            i++;
        }
        

        free(line_copy);
        free(line);
    }

    print_list(&list, FALSE);
    free_list(&list);


    return 0;
}