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

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
        handleErrors();

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
        handleErrors();

    if(1 != EVP_DecryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    if(1 != EVP_DecryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int main() {
    unsigned char *key = (unsigned char *)"0123456789012345";
    unsigned char plaintext[256] = "";
    unsigned char ciphertext[256]; 
    unsigned char restored[256]; 
    int cipher_len = 0;
    int decrypted_len = 0;

	while(1)
	{
		fgets((char *)plaintext, sizeof(plaintext), stdin);
		plaintext[strcspn((char *)plaintext, "\n")] = 0;
		
		if (strcmp((char *)plaintext, "EXIT") == 0) {
			printf("\n[SECURE LINE TERMINATED]\n");
			break;
		}
		else {
			cipher_len = encrypt(plaintext, strlen((char *)plaintext), key, ciphertext);
			printf("[NETWORK SEND]: ");
			for(int i = 0; i < cipher_len; i++) {
				printf("%02x", ciphertext[i]);
			}
			printf("\n");
			
			decrypted_len = decrypt(ciphertext, cipher_len, key, restored);
			restored[decrypted_len] = '\0';
			printf("[NETWORK RECV]: %s\n", restored);
		}
	}
    return 0;
}
