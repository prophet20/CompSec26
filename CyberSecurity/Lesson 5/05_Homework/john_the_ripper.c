#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <openssl/evp.h>

char attack[20] = "НЕРАЗБИТА";
int users[50] = {0};

// SHA-256 HASHING
void calculate_sha256(const unsigned char *message, size_t len, unsigned char *digest, unsigned int *digest_len) {
    EVP_MD_CTX *mdctx;

    if((mdctx = EVP_MD_CTX_new()) == NULL) {
        printf("Error creating context\n");
        return;
    }

    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, message, len);
    EVP_DigestFinal_ex(mdctx, digest, digest_len);
    EVP_MD_CTX_free(mdctx);
}

// BRUTE FORCE ATTACK
void crack_pin(const char *user, int user_id, const char *target_hash) {
    char guess[5];
    unsigned char digest[32];
    unsigned int digest_len;
    char hex_output[65];

    for (int i = 0; i <= 9999; i++) {
        sprintf(guess, "%04d", i);

        calculate_sha256((unsigned char*)guess, strlen(guess), digest, &digest_len);

        for(int j = 0; j < 32; j++) {
            sprintf(&hex_output[j*2], "%02x", digest[j]);
        }
        hex_output[64] = '\0';

        if (strcmp(hex_output, target_hash) == 0) {
			users[user_id] = 1;
            printf("%s: %s (brute-force)\n", user, guess);
            return;
        }
    }
    
    if (users[user_id] == 0) printf("%s: [НЕРАЗБИТА]\n", user);
}

// DICTIONARY ATTACK
void dictionary_attack(const char *user, int user_id, const char *target_hash, const char *dict_file) {
    FILE *file = fopen(dict_file, "r");
    if (!file) {
        printf("Грешка при отваряне на речника!\n");
        return;
    }

    char word[256];
    unsigned char digest[32];
    unsigned int digest_len;
    char hex_output[65];

    while (fgets(word, sizeof(word), file)) {
        word[strcspn(word, "\n")] = 0; 
        
        calculate_sha256((unsigned char*)word, strlen(word), digest, &digest_len);
        
        for(int j = 0; j < 32; j++) {
            sprintf(&hex_output[j*2], "%02x", digest[j]);
        }
        hex_output[64] = '\0';

        if (strcmp(hex_output, target_hash) == 0) {
			users[user_id] = 1;
            printf("%s: %s (dictionary)\n", user, word);
            fclose(file);
            return;
        }
    }
    if (users[user_id] == 0) printf("%s: [НЕРАЗБИТА]\n", user);
    fclose(file);
}

int main() {
    FILE *shadow = fopen("shadow.txt", "r");
    char line[500], user[50], hash[100];

    if (!shadow) return 1;
	
	printf("\n-- РЕЗУЛТАТИ --\n");
	int i = 0;
    while (fgets(line, sizeof(line), shadow)) {
        sscanf(line, "%49[^:]:%99s", user, hash);
        dictionary_attack(user, i, hash, "dictionary.txt");
        crack_pin(user, i, hash);
        i++;
    }
    fclose(shadow);
    return EXIT_SUCCESS;
}
