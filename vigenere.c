#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void vigenere_encrypt(const char *plaintext, const char *key, char *output)
{
    int key_len = strlen(key);
    int key_index = 0;

    for(int i = 0; plaintext[i] != '\0'; i++)
    {
        char p = toupper(plaintext[i]);
        
        // 1) SHIFT
        if(isalpha(p))
        {
            char k_char = toupper(key[key_index % key_len]);
            int shift = k_char - 'A';
        
        // TODO: 2) ENCRYPT
        // (A + L) % 26 
        // (0 + 11) % 26 = 11 (L)
        output[i] = ((p - 'A' + shift) % 26) + 'A';

        // TODO: 3) NEXT LETTER
        key_index++;
        } else {
            output[i] = plaintext[i];
        }
    }
    output[strlen(plaintext)] = '\0';
}

int main()
{
    /*
    TEXT - ATTACK AT DAWN
    KEY - LEMON
    EXPECTED RESULT - LXFOPV EF RNHR
    */
	char str[] = "ATTACK AT DAWN";
	char key[] = "LEMON";
	char output[100] = "";
    
    vigenere_encrypt("ATTACK AT DAWN", "LEMON", output);
    
    printf("%s", output);

    return EXIT_SUCCESS;
}
