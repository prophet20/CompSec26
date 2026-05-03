#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void vigenere_encrypt(const char *plaintext, const char *key, char *output, char *mode)
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
        if (mode == "encrypt") {
			output[i] = ((p - 'A' + shift) % 26) + 'A';
		} 
		else if (mode == "decrypt") {
			output[i] = ((p - 'A' - shift + 26) % 26) + 'A';
		}
        
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
    
    // READ ENCRYPTED TEXT
    FILE *strFILE;
    strFILE = fopen("encrypted.txt", "r");
	char str[100];
	fscanf(strFILE, "%99[^\n]", str);
    fclose(strFILE);
    printf("%s\n",str);
    
    // READ KEY
    FILE *keyFILE;
    keyFILE = fopen("key.txt", "r");
	char key[100];
	fscanf(keyFILE, "%99[^\n]", key);
    fclose(keyFILE);
    printf("%s\n",key);
	
	// Encryption/ Decryption
	char output[100] = "";
    
    vigenere_encrypt(str, key, output, "encrypt");
    printf("%s\n", output);
    
    vigenere_encrypt(output, key, output, "decrypt");
    printf("%s\n", output);

    return EXIT_SUCCESS;
}
