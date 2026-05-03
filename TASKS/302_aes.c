#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>

void handleErrors() {
    printf("An error occurred within OpenSSL logic.\n");
    exit(1);
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    // 1. Създаване и инициализиране на контекста
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // 2. Инициализиране на криптирането.
    // Използваме AES-128-ECB. (ECB не ползва инициализационен вектор, затова последният параметър е NULL)
    // TODO: Извикайте EVP_EncryptInit_ex
    // Параметри: ctx, EVP_aes_128_ecb(), NULL, key, NULL
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
        handleErrors();

    // 3. Криптиране на съобщението (Update)
    // Тази функция чете 'plaintext_len' байта и пише в 'ciphertext'.
    // Броят на записаните байтове се връща в 'len'.
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    // 4. Финализиране (Final)
    // Тази стъпка е КРИТИЧНА! Тя добавя Padding (PKCS#7) и криптира последния блок.
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    // 5. Почистване на паметта
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int main() {
    printf("LESSON 3, TASK 2 - Програмиране на AES с OpenSSL (EVP API) (aes.c)\n\n");
    
    unsigned char *key = (unsigned char *)"0123456789012345";
    unsigned char *plaintext = (unsigned char *)"CyberSecurity Lab";
    unsigned char ciphertext[128]; 

    printf("Original Text: %s\n", plaintext);

    int cipher_len = encrypt(plaintext, strlen((char *)plaintext), key, ciphertext);

    printf("Ciphertext is (%d bytes): ", cipher_len);
    for(int i = 0; i < cipher_len; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}
