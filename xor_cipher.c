#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* xor_cipher(char *text, char key)
{
    int len = strlen(text);
    char* result = malloc(sizeof(char)*len+1);
    for (int i = 0; i < len; i++)
    {
        // TODO: Приложете XOR операция върху text[i]
        result[i] = text[i] ^ key;
    }
    result[len] = '\0';
    return result;

}

int main()
{
    char data[] = "Secret";
    char key = 'K';
    char* encrypted = xor_cipher(data, key);
    printf("Encrypted: %s\n", encrypted); // Внимание: Може да съдържа невидими символи!
    char* decrypted = xor_cipher(encrypted, key);
    printf("Decrypted: %s\n", decrypted);

    return EXIT_SUCCESS;
}

