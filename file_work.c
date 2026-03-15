#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main()
{
	char c = 'X';
	char buffer[100];
	
	FILE *inputFILE;
    inputFILE = fopen("input.txt", "r");
	fscanf(inputFILE, "%99[^\n]", buffer);
    fclose(inputFILE);
    printf("Plaintext: %s\n",buffer);

	for (int i = 0; i < strlen(buffer); i++)
    {
		buffer[i] = buffer[i] ^ c;
	}
	//printf("%s\n",buffer);
	
	FILE *outputFILE;
    outputFILE = fopen("output.txt", "w");
	fprintf(outputFILE, buffer);
    fclose(outputFILE);
    
    printf("Encrypted text: %s",buffer);
	
    return EXIT_SUCCESS;
}
