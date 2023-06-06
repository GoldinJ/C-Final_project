#include <stdio.h>
#include <stdlib.h>

char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* convertToBase64(int binary) {
    char* base64 = malloc(sizeof(char) * 3);
    base64[0] = base64_table[(binary >> 6) & 0x3F];
    base64[1] = base64_table[binary & 0x3F];
    base64[2] = '\0';
    return base64;
}

int main() {
    int binary_number = 2572;
    char* base64 = convertToBase64(binary_number);
    printf("Base64 representation: %s\n", base64);
    free(base64);
    return 0;
}