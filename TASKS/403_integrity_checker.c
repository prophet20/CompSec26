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

int main() {
    printf("LESSON 4, TASK 3 - Проверка на интегритет (Integrity Checker)\n\n");
    
    unsigned char stored_hash[] = "8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918";
    unsigned char password[100];
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    
    char hash_string[65];
    
    printf("Enter password: ");
    scanf("%s", password);

    calculate_sha256(password, hash, &hash_len);

	// --------------------------------------------
    printf("Stored hash: %s\n", stored_hash);
    printf("SHA-256 Digest: ");
    for(int i = 0; i < hash_len; i++) {
        // Записваме всеки байт като 2 hex символа в низа
        // Използваме отместване (i * 2), за да не презаписваме началото
        sprintf(hash_string + (i * 2), "%02x", hash[i]);
        
        // Принтираме за проверка в конзолата
        printf("%02x", hash[i]);
    }
    hash_string[64] = '\0'; // Терминираме низа
    printf("\n");

    printf("Entered hash: %s\n", hash_string);
    if (strcmp((char *)stored_hash, hash_string) == 0) printf("\nAccess Granted!\n");
    else printf("\nAccess Denied!\n");

    return EXIT_SUCCESS;
}
