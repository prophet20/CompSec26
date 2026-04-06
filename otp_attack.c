#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    unsigned char c1[] = {0x53, 0x40, 0x22, 0x19, 0xF9, 0xD7}; // Encrypted "Attack"
    unsigned char c2[] = {0x56, 0x51, 0x30, 0x1D, 0xF4, 0xD8}; // Encrypted secret message
    
    char m1_guess[] = "Attack"; 
    char m2_recovered[7];

   int m1_len = strlen(m1_guess);

    printf("Attempting to recover M2...\n");
    
    for(int i = 0; i < m1_len; i++) {
        m2_recovered[i] = c1[i] ^ c2[i] ^ m1_guess[i];
    }
    m2_recovered[m1_len] = '\0'; // Null-terminate
    
    printf("Recovered Message 2: %s\n", m2_recovered);
    return 0;
}