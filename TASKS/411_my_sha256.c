#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>

// Дефинираме размер на буфера (1KB според условието)
#define BUFF_SIZE 1024

void calculate_sha256(FILE *inFile, unsigned char *digest, unsigned int *digest_len) {
    EVP_MD_CTX *mdctx;
    unsigned char buff[BUFF_SIZE]; // Локален буфер за четене
    size_t n;

    if((mdctx = EVP_MD_CTX_new()) == NULL) {
        printf("Error creating context\n");
        return;
    }

    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);

    // Четем файла на части (buffer loop)
    while ((n = fread(buff, 1, sizeof(buff), inFile)) > 0) {
        EVP_DigestUpdate(mdctx, buff, n);
    }

    EVP_DigestFinal_ex(mdctx, digest, digest_len);
    EVP_MD_CTX_free(mdctx);
}

int main(int argc, char *argv[]) {
    // Проверка дали е подадено име на файл като аргумент
    if (argc < 2) {
        printf("Употреба: %s input_file\n", argv[0]);
        return 1;
    }
    
    // Отваряме файла, подаден от командния ред (argv[1])
    FILE *inFile = fopen(argv[1], "rb");
    if (!inFile) {
        printf("Грешка при отваряне на файла: %s\n", argv[1]);
        return 1;
    }
    
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    // Извикваме функцията с указателя към файла
    calculate_sha256(inFile, hash, &hash_len);
    
    fclose(inFile); // Затваряме файла веднага след работа

    // Принтиране на резултата в HEX формат
    printf("SHA-256 Digest: ");
    for(int i = 0; i < hash_len; i++) {
        printf("%02x", hash[i]);
    }
    printf("  %s\n", argv[1]);

    return EXIT_SUCCESS;
}
