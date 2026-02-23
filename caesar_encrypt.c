#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // за isupper()
#define MAXN 1000

char caesar_encrypt_char(char p, int k) {
    if (isupper(p)) {
        // TODO: 1. Превърнете p в индекс 0-25 (извадете 'A')
        // TODO: 2. Добавете ключа k
        // TODO: 3. Вземете модул 26
        // TODO: 4. Върнете обратно към ASCII (добавете 'A')
        return ( (p - 'A' + k) % 26 ) + 'A';
    }
    return p; // Ако не е буква (напр. цифра), връщаме без промяна
}

char caesar_decrypt_char(char c, int k) {
    if (isupper(c)) {
        return ( (c - 'A' - k + 26) % 26) + 'A';
        // TODO: Реализирайте обратната логика.
        // ВАЖНО: Използвайте (x - k + 26) % 26
    }                           
    return c;
}


int main()
{
    char plaintext[MAXN];
    scanf("%s", plaintext);
    int key;
    scanf("%d", &key);
 
    /*for (int i = 0; i <= 10; i++){
        printf(caesar_encrypt_char(plaintext[i],key));
    }

    for (int i = 0; i <= 10; i++){
        printf(caesar_decrypt_char(plaintext[i],key));
    }*/

    char* cipher = caesar_encrypt_char(plaintext, key);
    printf(cipher);
    return EXIT_SUCCESS;
}