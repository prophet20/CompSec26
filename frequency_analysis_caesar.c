#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main()
{
    printf("\n");
    char plaintext[1000] = "This is a test message to count letters";
    int counts[26] = {0};
    int total_letters = 0;

    printf("%s\n", plaintext);

    for(int i = 0; i < strlen(plaintext); i++)
    {
        if(isalpha(plaintext[i]))
        {
            int index = toupper(plaintext[i]) - 'A';
            counts[index]++;
            total_letters++;
        }   
    }
    
    for(int i = 0; i < 26; i++)
    {
        int index = toupper(plaintext[i]) - 'A';
        printf("%c - ",(i + 'A'));
        for (int j = 0; j < counts[i]; j++){
            printf("*");
        }
        printf("\n");
        //printf("%c: %d\n",(i + 'A'), counts[i]);
    }

    return EXIT_SUCCESS;
}