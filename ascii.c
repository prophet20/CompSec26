#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_hex(const char *text) {
    printf("Symbol | Decimal | Hex\n");
    printf("-------|---------|-----\n");
    
    for (int i = 0; i < strlen(text); i++) {
        // TODO: Използвайте printf с формати %c, %d и %02X
        printf("%6c | %7d | %02x\n", text[i], text[i], text[i]);
    }
}

int main() {
    char message[] = "Cyber";
    print_hex(message);
    return EXIT_SUCCESS;
}