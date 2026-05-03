#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

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

int main() {
    char choice[10];
    char username[50];
    char password[50];
	unsigned char salt[16];
	unsigned char salt_password[100] = {0};
	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hash_len;
	unsigned char hash_string[65];
	char stored_user[50], stored_salt[33], stored_hash[65];
	unsigned char line[500];

	FILE *users;

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Quit.\n");
        printf("Choose action: ");
        
        scanf("%s", choice); // Четем избора

        if (strcmp(choice, "3") == 0) {
            break; // Излизаме от цикъла
        }

        if (strcmp(choice, "1") == 0) {
            // ЛОГИКА ЗА РЕГИСТРАЦИЯ
            printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);
            
            // Тук ще генерираш солта и ще смяташ хеша
            printf("Registering user %s...\n", username);
            
			RAND_bytes(salt, sizeof(salt));
			memset(salt_password, 0, sizeof(salt_password));
			memcpy(salt_password, salt, sizeof(salt));
			memcpy(salt_password + sizeof(salt), password, strlen(password));
           
			size_t total_len = sizeof(salt) + strlen(password);
			calculate_sha256(salt_password, total_len, hash, &hash_len);
           
			// В режим "Register", след като генерираш salt:
			char salt_hex[33]; // 16 байта * 2 символа + 1 за '\0'
			// 1. ПРЕВРЪЩАШ ХЕША В ТЕКСТ (Това липсваше!)
for(int i = 0; i < hash_len; i++) {
    sprintf(hash_string + (i * 2), "%02x", hash[i]);
}
hash_string[hash_len * 2] = '\0'; // Задължително терминираш низа[cite: 4]

// 2. Превръщаш солта в текст
for(int i = 0; i < 16; i++) {
    sprintf(salt_hex + (i * 2), "%02x", salt[i]);
}

// 3. Записваш вече ПРАВИЛНИТЕ текстови низове
users = fopen("users.txt", "a");
fprintf(users, "%s:%s:%s\n", username, salt_hex, hash_string);
fclose(users);
           
        } 
        
        else if (strcmp(choice, "2") == 0) {
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    users = fopen("users.txt", "r");
    if (!users) { printf("No users database found.\n"); continue; }

    int found = 0;
    while(fgets(line, sizeof(line), users)) {
        sscanf(line, "%49[^:]:%33[^:]:%65s", stored_user, stored_salt, stored_hash);
        stored_hash[strcspn(stored_hash, "\n")] = 0;

        if (strcmp(username, (char*)stored_user) == 0) {
            found = 1;
            
            // 1. Превръщаме stored_salt (Hex текст) обратно в бинарни байтове
            unsigned char binary_salt[16];
            for (int i = 0; i < 16; i++) {
                sscanf((char*)stored_salt + (i * 2), "%02hhx", &binary_salt[i]);
            }

            // 2. Съединяваме дешифрираната сол с въведената парола
            unsigned char check_buffer[100] = {0};
            memcpy(check_buffer, binary_salt, 16);
            memcpy(check_buffer + 16, password, strlen(password));

            // 3. Смятаме хеша на тази комбинация
            unsigned char login_hash[EVP_MAX_MD_SIZE];
            unsigned int login_hash_len;
            size_t total_len = 16 + strlen(password);
			calculate_sha256(check_buffer, total_len, login_hash, &login_hash_len);

            // 4. Превръщаме новия хеш в Hex низ за сравнение
            char login_hash_string[129] = {0};
            for(int i = 0; i < login_hash_len; i++) {
                sprintf(login_hash_string + (i * 2), "%02x", login_hash[i]);
            }

			printf("Stored: %s\n", stored_hash);
			printf("Login : %s\n", login_hash_string);

            // 5. Сравняваме двата Hex низа
            if (strcmp(login_hash_string, (char*)stored_hash) == 0) {
                printf("--- LOGIN SUCCESSFUL! Welcome, %s ---\n", username);
            } else {
                printf("--- LOGIN FAILED! Wrong password. ---\n");
            }
            break; 
        }
    }
    if (!found) printf("User not found.\n");
    fclose(users);
	}
        else {
            printf("Invalid choice!\n");
        }
    }

    printf("Goodbye!\n");
    return 0;
}
