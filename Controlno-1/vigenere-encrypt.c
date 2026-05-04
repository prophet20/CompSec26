#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char vigenere_encrypt(char *plaintext, char *key, char *output)
{
    int key_len = strlen(key);
    int key_index = 0;
    char a = '0';

    for(int i = 0; plaintext[i] != '\0'; i++)
    {
        char p = plaintext[i];
        
        if(isalpha(p))
        {
            if (isupper(p)) a = 'A';
            else if (islower(p)) a = 'a';
            
            char k_char = key[key_index % key_len];
            int shift = toupper(k_char - a);
			output[i] = ((p - a + shift) % 26) + a;

        key_index++;
        
        } else {
            output[i] = plaintext[i];
        }
    }
    output[strlen(plaintext)] = '\0';
}

int main(int argc, char *argv[])
{
   if (argc < 3) 
   {
    printf("Use <input> <key> <output>\n");
   }

   char *inputFileName = argv[1];
   char *keyFileName = argv[2];
   char *outputFileName = argv[3];
    
   
    FILE *textFile;
    textFile = fopen(inputFileName, "r");
    char text[100];
    fscanf(textFile, "%99[^\n]", text);
    fclose(textFile);

    FILE *keyFile;
    keyFile = fopen(keyFileName, "r");
    char key[100];
    fscanf(keyFile, "%99[^\n]", key);
    fclose(keyFile);

    if (!textFile || !keyFile) {
        printf("Error!\n");
    }

    char output[100];
    vigenere_encrypt(text, key, output);

    printf("%s", output);

    FILE *outFile;
    outFile = fopen(outputFileName, "w");
    fprintf(outFile, output, 100);
    fclose(outFile);

    return EXIT_SUCCESS;
}