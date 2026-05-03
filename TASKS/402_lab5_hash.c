#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

void calculate_sha256(const char *message, unsigned char *digest, unsigned int *digest_len) {
    EVP_MD_CTX *mdctx;

    // 1. Създаване на контекст
    if((mdctx = EVP_MD_CTX_new()) == NULL) {
        printf("Error creating context\n");
        return;
    }

    // 2. Инициализация (Избираме SHA-256)
    // EVP_sha256() е модерният начин, замества старите SHA256_Init
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);

    // 3. Добавяне на данните (Update)
    // Може да се вика много пъти, ако четем файл на части
    EVP_DigestUpdate(mdctx, message, strlen(message));

    // 4. Финализиране (Final)
    // Записва резултата в 'digest'
    EVP_DigestFinal_ex(mdctx, digest, digest_len);

    // 5. Почистване
    EVP_MD_CTX_free(mdctx);
}

void calculate_md5(const char *message, unsigned char *digest, unsigned int *digest_len) {
    EVP_MD_CTX *mdctx;

    // 1. Създаване на контекст
    if((mdctx = EVP_MD_CTX_new()) == NULL) {
        printf("Error creating context\n");
        return;
    }

    // 2. Инициализация (Избираме SHA-256)
    // EVP_sha256() е модерният начин, замества старите SHA256_Init
    EVP_DigestInit_ex(mdctx, EVP_md5(), NULL);

    // 3. Добавяне на данните (Update)
    // Може да се вика много пъти, ако четем файл на части
    EVP_DigestUpdate(mdctx, message, strlen(message));

    // 4. Финализиране (Final)
    // Записва резултата в 'digest'
    EVP_DigestFinal_ex(mdctx, digest, digest_len);

    // 5. Почистване
    EVP_MD_CTX_free(mdctx);
}

int main() {
    printf("LESSON 4, TASK 2 - Програмиране на SHA-256 (EVP Interface) (lab5_hash.c)\n\n");
    
    char message[] = "CyberSecurity";
    unsigned char hash[EVP_MAX_MD_SIZE]; // Буфер за бинарния хеш
    unsigned int hash_len;

    printf("Original Text: %s\n", message);

    // Извикване на функцията
    calculate_sha256(message, hash, &hash_len);

    // ПРИНТИРАНЕ (Bin -> Hex)
    printf("SHA-256 Digest: ");
    for(int i = 0; i < hash_len; i++) {
        // %02x принтира всеки байт като 2 hex цифри
        printf("%02x", hash[i]);
    }
    printf("\n");
    
    calculate_md5(message, hash, &hash_len);

    // ПРИНТИРАНЕ (Bin -> Hex)
    printf("MD5 Digest: ");
    for(int i = 0; i < hash_len; i++) {
        // %02x принтира всеки байт като 2 hex цифри
        printf("%02x", hash[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}
