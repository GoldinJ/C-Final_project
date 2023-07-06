#include "utilities.h"


char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* int_to_Base64(int binary) {
    char* base64 = malloc(sizeof(char) * 3);
    base64[0] = base64_table[(binary >> 6) & 0x3F];
    base64[1] = base64_table[binary & 0x3F];
    base64[2] = '\0';
    return base64;
}

void print_last_12_bits(unsigned int num) {
    int mask = (1 << 12) - 1;  
    int last12Bits = num & mask;
    int i;
    int bit;

    for (i = 11; i >= 0; i--) {
        bit = (last12Bits >> i) & 1;
        printf("%d", bit);
    }
    printf("\n");
}