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

int sizeof_instruction(char **line){
    int i = 0;
    while (line[i] != NULL)
    {   
        i++;
    }
    
    return i;
}

int get_token_type(char *token){
    int i;

    if(END_WITH(token, ':')){ /*Label syntax validation*/
        if (strlen(token) > 31){
            fprintf(stderr, INVALID_LABEL_LENGTH, token);
            return TOKEN_UNDEFINED;
        }

        if(!IS_CHAR(token[0])){
            printf("Error:\n");
            return TOKEN_UNDEFINED;
        }
        
        for(i=1; i<strlen(token) - 1; i++){
            if(!IS_CHAR(token[i]) && !IN_RANGE(token[i], '0', '9')){
                fprintf(stderr, INVALID_LABEL_SYNTAX, token[i]);
                return TOKEN_UNDEFINED;
            }
        }
            
        return TOKEN_LABEL;
    }

    else if(is_in(token, opcodes))
        return TOKEN_OPCODE;

    else if(is_in(token, registers))
        return TOKEN_REGISTER;

    else if(is_in(token, data_types)){
        i = 0;
        while (data_types[i] != NULL)
        {
            if(strcmp(token, data_types[i]) == 0)
                break;

            i++;
        }

        switch (i)
            {
            case ENTRY:
                return TOKEN_ENTRY;
                break;

            case EXTERN:
                return TOKEN_EXTERN;
                break;

            case DATA:
                return TOKEN_DATA;
                break;

            case STR:
                return TOKEN_STRING;
                break;
            
            default:
                break;
            }
        
    }
        
    else if(is_num(token))
        return TOKEN_INEGER;

    else if (START_WITH(token, '"') && END_WITH(token, '"'))
        return TOKEN_STRING;

    else
        return TOKEN_VAR;

    return TOKEN_UNDEFINED;
}

void allocate_word_queue(machine_w*** word, int size){
    /*size = first_w + num of operands* + NULL terminator*/

    int i;
    *word = (machine_w**)realloc(*word, size*(sizeof(machine_w*)));
    for(i = 1; i<size-1; i++)
        (*word)[i] =(machine_w*)malloc(sizeof(machine_w));

    (*word)[i] = NULL;
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

machine_w** encode_first_w(char** line, char* label, int opcode_index, int start_index){

    int i = start_index;
    int initial_queue_size = 2;
    machine_w** word_queue = (machine_w**)malloc(sizeof(machine_w*));
    first_w* f_word = (first_w*)malloc(sizeof(first_w));

    word_queue[0] = (machine_w*)malloc(sizeof(machine_w));
    word_queue[0]->label = label;
    word_queue[0]->node_type = NODE_FIRST_W;
    word_queue[0]->word.f_word = f_word;
    f_word->ARE = A;

    switch (opcode_index)
        {
            case CMP:
            case MOV:
            case ADD:
            case SUB:
            case LEA:

                if(line[i] == NULL || line[i+1] ==NULL){
                    printf(TOO_FEW_OPERANDS, opcodes[opcode_index]);
                    free(f_word);
                    return NULL;
                }

                allocate_word_queue(&word_queue, initial_queue_size+2);
                
                f_word->opcode = opcode_index;
                
                /*First operand is an integer*/
                if(is_num(line[i])){
                    f_word->src_operand = IMDT;
                    word_queue[1] = encode_imdt_drct_w(line[i], FALSE);
                    i++;
                }

                /*Both operands are registers*/
                else if(get_reg_index(line[i]) != TOKEN_UNDEFINED && get_reg_index(line[i+1]) != TOKEN_UNDEFINED){
                    f_word ->src_operand = DRCT_REG;
                    f_word ->dest_operand = DRCT_REG;
                    word_queue[0]->word.f_word = f_word;
                    word_queue[1] = encode_reg_w(line[i], line[i+1]);
                    free(word_queue[2]);
                    free(word_queue[3]);
                    word_queue[2] = NULL;
                    break;
                }

                /*First operand is a register*/
                else if (get_reg_index(line[i]) != TOKEN_UNDEFINED)
                {
                    f_word->src_operand = DRCT_REG;
                    word_queue[1] = encode_reg_w(line[i], NULL);
                    i++;
                }
                else{
                    f_word->src_operand = DRCT;
                    word_queue[1] = encode_imdt_drct_w(line[i], TRUE);
                    i++;
                }
            
               /*The second operand is an integer - invalid for any opcode except CMP*/
                if(is_num(line[i]) && opcode_index != CMP){
                    printf(INVALID_OPERAND_TYPE, opcodes[opcode_index], i);
                    free(word_queue[0]);
                    free(word_queue[1]);
                    free(word_queue[2]);
                    free(word_queue[3]);
                    return NULL;
                }

                else if(is_num(line[i]) && opcode_index == CMP){
                    f_word->dest_operand = IMDT;
                    word_queue[2] = encode_imdt_drct_w(line[i], FALSE);
                }
                /*Second operand is a register*/
                else if (get_reg_index(line[i]) != TOKEN_UNDEFINED)
                {
                    f_word->dest_operand = DRCT_REG;
                    word_queue[2] = encode_reg_w(NULL, line[i]);
                }
                
                else{
                    f_word->dest_operand = DRCT;
                    word_queue[2] = encode_imdt_drct_w(line[i], TRUE);
                }
               
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

                allocate_word_queue(&word_queue, initial_queue_size+1);
                f_word->opcode = opcode_index;

                if(line[i] == NULL){
                    printf(TOO_FEW_OPERANDS, opcodes[opcode_index]);
                    return NULL;
                }

                else if(is_num(line[i])){
                    f_word->dest_operand = IMDT;
                    word_queue[1] = encode_imdt_drct_w(line[i], FALSE);
                }
                else if (get_reg_index(line[i]) != TOKEN_UNDEFINED)
                {
                    f_word->dest_operand= DRCT_REG;
                    word_queue[1] = encode_reg_w(line[i], NULL);
                }
                else
                    f_word->dest_operand = DRCT;
                    word_queue[1] = encode_imdt_drct_w(line[i], TRUE);
                
                break;

            case RTS:
            case STOP:
                if(line[i] != NULL){
                    printf(TOO_MANY_OPERANDS, opcodes[opcode_index]);
                    return NULL;
                    break;
                }

                f_word->src_operand = 0;
                f_word->opcode = opcode_index;
                f_word->dest_operand = 0;
                break;
            
            default:
                printf("Error: Undefined opcode - '%s'\n", line[i]);
                free(word_queue);
                free(f_word);
                return NULL;
                break;
        }

    return word_queue;
}

machine_w** encode_data_w(char **line, char *label, int token_type, int start_index){
    int j;
    int queue_size;
    char *token;
    int num;
    machine_w** word_queue = (machine_w**)malloc(sizeof(machine_w*));
    data_w *d_word;
    
    switch (token_type)
    {

    case TOKEN_DATA:
        queue_size = sizeof_instruction(line) - start_index + 1;
        allocate_word_queue(&word_queue, queue_size);
        word_queue[0] = (machine_w*)malloc(sizeof(machine_w));
        for(j=0; j<queue_size-1; j++, start_index++){
            token = line[start_index];
            d_word = (data_w*)malloc(sizeof(d_word));

            if(is_num(token)){
                num = to_int(token);
                d_word->data = num;
                word_queue[j]->word.d_word = d_word;

                if(j == 0)
                    word_queue[j]->label = label;
            }

        }

        return word_queue;

    case TOKEN_STRING:
        token = line[start_index];
        queue_size = sizeof(token);
        allocate_word_queue(&word_queue, queue_size);
        word_queue[0] = (machine_w*)malloc(sizeof(machine_w));

        for(j=1; j<queue_size-1; j++){
            d_word = (data_w*)malloc(sizeof(d_word));
            d_word->data = token[j];

            if(j==1){
                word_queue[0]->label = label;
                d_word->data = token[j];
                word_queue[0]->word.d_word = d_word;
            }
    
            else{
                d_word->data = token[j];
                word_queue[j-1]->word.d_word = d_word;
            }

        }

        d_word = (data_w*)malloc(sizeof(d_word));
        d_word->data = 0;
        word_queue[j-1]->word.d_word = d_word;

        return word_queue;
    
    default:
        break;
    }
    
    return NULL;
}

machine_w** encode(char** instruction){
    int i;
    char *token;
    int label_size;
    char *label = NULL;
    int token_type;

    for(i = 0; i<2; i++){
        token = instruction[i];
        token_type = get_token_type(token);

        switch (token_type)
        {
        case TOKEN_LABEL:
            label_size = strlen(instruction[0]);
            label = malloc((label_size-1)*sizeof(char*));
            strncpy(label, instruction[0], label_size-1);
            label[label_size-1] = '\0';
            continue;

        case TOKEN_OPCODE:
            return encode_first_w(instruction, label, get_opcode_index(token), i+1);

        case TOKEN_DATA:
        case TOKEN_STRING:
            return encode_data_w(instruction, label, token_type, i+1);
        
        case TOKEN_UNDEFINED:
            return NULL;
        }

    }
    

    return NULL;

}

int main (){

    char *line;
    char *line_copy;
    char **instruction = NULL;
    int line_len;
    int i;
    machine_w* tmp;
    machine_w** word_queue;

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
        word_queue = encode(instruction);
        
        if (word_queue == NULL){
            free(line_copy);
            free(line);
            continue;
        }

        i = 0;
        while(word_queue[i] != NULL){
            tmp = word_queue[i];
            if (i == 0)
                add_node(&list, instruction, tmp);
            else
                add_node(&list, NULL, tmp);
            i++;
        }
        
        free(word_queue);
        free(line_copy);
        free(line);
    }

    print_list(&list, FALSE);
    free_list(&list);


    return 0;
}