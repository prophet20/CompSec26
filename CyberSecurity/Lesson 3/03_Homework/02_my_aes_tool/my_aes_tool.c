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
    int len, ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL)) handleErrors();

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) handleErrors();
    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len, plaintext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL)) handleErrors();

    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) handleErrors();
    plaintext_len = len;

    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Употреба: %s [enc|dec] input_file output_file key_string\n", argv[0]);
        return 1;
    }

    char *mode = argv[1];
    FILE *inFile = fopen(argv[2], "r"); // Бинарно четене
    FILE *outFile = fopen(argv[3], "wb"); // Бинарно писане
    
    if (!inFile || !outFile) {
        printf("Грешка при отваряне на файловете!\n");
        return 1;
    }

    unsigned char key[16];
    memset(key, 0, 16); 
    strncpy((char *)key, argv[4], 16);

    unsigned char in_data[1024];
    unsigned char out_data[1024 + 16];
    
    int bytes_read = fread(in_data, 1, 1024, inFile);
    int final_len = 0;

    if (strcmp(mode, "enc") == 0) {
        final_len = encrypt(in_data, bytes_read, key, out_data);
    } 
    else if (strcmp(mode, "dec") == 0) {
        final_len = decrypt(in_data, bytes_read, key, out_data);
    } 
    else {
        printf("Грешен режим!\n");
        return 1;
    }

    fwrite(out_data, 1, final_len, outFile);

    fclose(inFile);
    fclose(outFile);

    printf("Операцията [%s] завърши успешно.\n", mode);
    return 0;
}
