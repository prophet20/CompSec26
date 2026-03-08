#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // за isupper()
#define MAXN 1000

char caesar_encrypt_char(char p, int k) {
        if (isupper(p)) {
			return (p - 'A' + k) % 26 + 'A';
		} 
		else if (islower(p)) {
			return (p - 'a' + k) % 26 + 'a';
		} 
		else if (isdigit(p)) {
			return (p - '0' + k) % 10 + '0';
		}
    
		return p;
}

char caesar_decrypt_char(char c, int k) {
        if (isupper(c)) {
			return (((int)c - 'A' - k + 26) % 26 + 'A');
		} 
		else if (islower(c)) {
			return (((int)c - 'a' - k + 26) % 26 + 'a');
		} 
		else if (isdigit(c)) {
			return (((int)c - '0' - k + 26) % 26 + '0');
		}
    
		return c;
}

int main()
{
    char plaintext[MAXN];
    printf("Enter text to cipher: ");
    scanf("%s", plaintext);
    
    int key;
    printf("Enter key for cipher: ");
    scanf("%d", &key);
    
    char encrypted_text[MAXN];
    char decrypted_text[MAXN];
    
    for (int i = 0; i < strlen(plaintext); i++)
    { 
		encrypted_text[i] = caesar_encrypt_char(plaintext[i], key); 
	}
	encrypted_text[strlen(plaintext)] = '\0';
    printf("Encypted text: %s\n", &encrypted_text);
    
    
    for (int i = 0; i < strlen(encrypted_text); i++)
    { 
		decrypted_text[i] = caesar_decrypt_char(encrypted_text[i], key); 
	}
    decrypted_text[strlen(plaintext)] = '\0';
    printf("Decrypted text: %s\n", &decrypted_text);
    return EXIT_SUCCESS;
}
