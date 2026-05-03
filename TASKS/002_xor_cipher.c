#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *in = fopen("input.txt", "rb");
    FILE *out = fopen("encrypted.bin", "wb");
    
    char ch;
    char key = 'a';
    
    while((ch == fgetc(in)!=EOF))
    {
			// TODO
			char encrypted = ch ^ key;
			fputc(encrypted, out);
	}
	
	fclose(in);
	fclose(out);
    
    return EXIT_SUCCESS;
}
