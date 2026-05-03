#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // за isupper()
#define MAXN 1000

/* OLD FUNCTIONS
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
*/

char caesar_cipher(char *c, int k, char *output, int mode) {
		int a = '0';
		int num = 0;
		int len = strlen(c);
		for (int i = 0; i < len; i++) 
		{
			if (isupper(c[i])) { a = 'A'; num = 26;}
			else if (islower(c[i])) { a = 'a'; num = 26;}
			else if (isdigit(c[i])) { a = '0'; num = 10;}
			
			if (mode == 0) output[i] = (c[i] - a + k) % num + a;
			else if (mode == 1) output[i] = (c[i]- a - k + num) % num + a;
			
		}
		output[len] = '\0';
}

int main()
{
    char plaintext[MAXN];
    printf("Enter text to cipher: ");
    scanf("%s", plaintext);
    
    int key;
    printf("Enter key for cipher: ");
    scanf("%d", &key);
    
	char output[100] = "";
	
	caesar_cipher(plaintext, key, output, 0);
    printf("Encypted text: %s\n", output);
	caesar_cipher(output, key, output, 1);
    printf("Decrypted text: %s\n", output);
    return EXIT_SUCCESS;
}
