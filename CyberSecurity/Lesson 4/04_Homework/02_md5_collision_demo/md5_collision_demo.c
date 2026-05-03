#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>

// Дефинираме размер на буфера (1KB според условието)
#define BUFF_SIZE 1024

//SHA_256
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

//MD5
void calculate_MD5(FILE *inFile, unsigned char *digest, unsigned int *digest_len) {
    EVP_MD_CTX *mdctx;
    unsigned char buff[BUFF_SIZE]; // Локален буфер за четене
    size_t n;

    if((mdctx = EVP_MD_CTX_new()) == NULL) {
        printf("Error creating context\n");
        return;
    }

    EVP_DigestInit_ex(mdctx, EVP_md5(), NULL);

    // Четем файла на части (buffer loop)
    while ((n = fread(buff, 1, sizeof(buff), inFile)) > 0) {
        EVP_DigestUpdate(mdctx, buff, n);
    }

    EVP_DigestFinal_ex(mdctx, digest, digest_len);
    EVP_MD_CTX_free(mdctx);
}

int main() {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    
    // Масиви за съхранение на резултатите
    unsigned char results_sha256[2][32]; 
    unsigned char results_md5[2][16];
    
    FILE *inFile;
    char *files[2] = {"message1.bin", "message2.bin"};

    for (int i = 0; i < 2; i++) {
        inFile = fopen(files[i], "rb");
        if (!inFile) {
            printf("Грешка при отваряне на %s\n", files[i]);
            return 1;
        }

        // 1. Смятаме SHA-256
        calculate_sha256(inFile, hash, &hash_len);
        memcpy(results_sha256[i], hash, 32);
        
        // Връщаме курсора в началото на файла за второто четене
        rewind(inFile); 

        // 2. Смятаме MD5
        calculate_MD5(inFile, hash, &hash_len);
        memcpy(results_md5[i], hash, 16);

        fclose(inFile);
    }

    // Принтиране и сравнение
    for (int i = 0; i < 2; i++) {
        printf("\nФайл: %s\n", files[i]);
        
        printf("MD5:    ");
        for(int j = 0; j < 16; j++) printf("%02x", results_md5[i][j]);
        
        printf("\nSHA-256: ");
        for(int j = 0; j < 32; j++) printf("%02x", results_sha256[i][j]);
        printf("\n");
    }

    // Проверка за колизия
    if (memcmp(results_md5[0], results_md5[1], 16) == 0) {
        printf("\n[!] MD5 Колизия открита! Хешовете са еднакви.\n");
    }
    if (memcmp(results_sha256[0], results_sha256[1], 32) != 0) {
        printf("[+] SHA-256 е сигурен: Хешовете са различни.\n");
    }

    return EXIT_SUCCESS;
}
