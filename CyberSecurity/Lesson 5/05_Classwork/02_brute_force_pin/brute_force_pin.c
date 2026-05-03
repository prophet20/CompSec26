#include <stdio.h>
#include <string.h>
#include <crypt.h> // Задължително!

void crack_pin(const char *target_hash) {
    char guess[5]; // 4 цифри + '\0'
    
    printf("--- Започва PIN Brute-Force ---\n");
    
    for (int i = 0; i <= 9999; i++) {
        // 1. Форматираме числото като 4-цифрен стринг (напр. 0042)
        sprintf(guess, "%04d", i);
        
        // 2. Хешираме предположението
        char *computed_hash = crypt(guess, target_hash);
        
        // 3. Сравняваме
        // TODO: Използвайте strcmp(). Ако върне 0, значи сме намерили паролата!
        if (strcmp(computed_hash, target_hash) == 0) {
            printf("[УСПЕХ] Паролата е: %s\n", guess);
            return;
        }
    }
    printf("[НЕУСПЕХ] Паролата не е 4-цифрен ПИН.\n");
}

int main() {
    // Поставяме точно изрязания хеш тук
    const char *target = "$6$xq7%3$03TJMkvoZmSoSVmWpDUuBvXNInUeIT3fLj6EvodCh1qFYjK9azoiyfxT4/gomNcpX.5Xfg08ZPU6U.xIHu8Pn."; 
    
    // Извикваме функцията за разбиване
    crack_pin(target);

    return 0;
}
