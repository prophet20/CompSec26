#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char shadow_line[] = "bob:$6$xyz123$T5x/L.C8vB/O:19000:0:99999:7:::";
    
    char *username = NULL;
    char *hash = NULL;

    // TODO: Използвайте strtok() за да разделите стринга по ":"
    // Първото извикване връща първия елемент (username)
    username = strtok(shadow_line, ":");
    
    // Второто извикване (с NULL) връща следващия елемент (hash)
    hash = strtok(NULL, ":");

    if (username && hash) {
        printf("Target User: %s\n", username);
        printf("Target Hash: %s\n", hash);
    } else {
        printf("Грешка при парсване!\n");
    }

    return EXIT_SUCCESS;
}
