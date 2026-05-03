#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // за isupper()

char caesar_decrypt_char(char c, int k) {
        if (isupper(c)) 
        {
			return (((int)c - 'A' - k + 26) % 26 + 'A');
		} 
		return c;
}

int main()
{
    char encrypted_text[] = "WKLV LV D VHFUHW PHVVDJH";
    char decrypted_text[strlen(encrypted_text)];
    
    for(int i = 1; i <= 25; i++)
    {
		for (int j = 0; j <= strlen(encrypted_text); j++)
		{ 
			decrypted_text[j] = caesar_decrypt_char(encrypted_text[j], i); 
		}
		printf("[%d]: %s\n", i, decrypted_text);
	}
    
    return EXIT_SUCCESS;
}

