#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char caesar_cipher(char *c, int k, char *output, int mode) {
		int a = '0';
		int num = 0;
		for (int i = 0; i < strlen(c); i++) 
		{
			if (isupper(c[i])) { a = 'A'; num = 26;}
			else if (islower(c[i])) { a = 'a'; num = 26;}
			else if (isdigit(c[i])) { a = '0'; num = 10;}
			
			if ( (isalpha(c[i]) || isdigit(c[i])) && mode == 1) 
			{
				output[i] = (c[i] - a + k) % num + a;
			}
			else if ((isalpha(c[i]) || isdigit(c[i])) && mode == -1) 
			{
				output[i] = (c[i]- a - k + num) % num + a;
			}
			else output[i] = c[i];
		}
		output[strlen(output)] = '\0';
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Употреба: %s -e/-d <input> <key> <output>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *flag = argv[1];
    char *inputFileName = argv[2];
    char *keyFileName = argv[3];
    char *outputFileName = argv[4];

    int mode = 0;
    if (strcmp(flag, "-e") == 0) {
        mode = 1;
    } else if (strcmp(flag, "-d") == 0) {
        mode = -1;
    } else {
        printf("Грешен флаг! Използвайте -e или -d.\n");
        return 1;
    }

    FILE *inFile = fopen(inputFileName, "r");
    FILE *keyFile = fopen(keyFileName, "r");
    FILE *outFile = fopen(outputFileName, "w");
	
    if (!inFile || !keyFile || !outFile) {
        printf("Грешка при отваряне на файловете!\n");
        return 1;
    }
	
	char plaintext[100];
	char output[100];
	fscanf(inFile, "%99[^\n]", plaintext);
	int key;
	fscanf(keyFile, "%d[^\n]", &key);
	

	caesar_cipher(plaintext, key, plaintext, mode);

	fprintf(outFile, plaintext);
	
    fclose(inFile);
    fclose(keyFile);
    fclose(outFile);

    printf("Операцията е завършена успешно в %s\n", outputFileName);

    return EXIT_SUCCESS;
}
