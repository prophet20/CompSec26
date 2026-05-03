#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>

// Набор от символи, които ще пробваме (за теста ползвайте само малки букви)
const char charset[] = "abcdefghijklmnopqrstuvwxyz0123456789";
const int charset_len = 36;
int is_found = 0; // Глобален флаг за спиране на рекурсията

// Функция за рекурсивно генериране
void generate_recursive(char *current_guess, int position, int max_length, const char *target_hash) {
    // Базови случаи:
    
    if (strcmp(current_guess, target_hash) == 0) {
		printf("[УСПЕХ] Паролата е разбита: %s\n", current_guess);
		is_found = 1;
	} else {
		printf("%s\n", current_guess);
	}
    
    if (is_found) return; // Спираме, ако вече сме я намерили
    
    // Проверка на текущата генерирана дума
    if (position > 0) {
        current_guess[position] = '\0'; // Затваряме стринга
        char *computed = crypt(current_guess, target_hash);
        if (strcmp(computed, target_hash) == 0) {
            printf("[УСПЕХ] Паролата е разбита: %s\n", current_guess);
            is_found = 1;
            return;
        }
    }

    // Ако сме стигнали максималната дължина, връщаме се назад
    if (position == max_length) return;

    // Рекурсивна стъпка: пробваме всеки символ за текущата позиция
    for (int i = 0; i < charset_len; i++) {
        // TODO: Сложете символ от charset на current_guess[position]
        current_guess[position] = charset[i];
        
        // TODO: Извикайте същата функция за следващата позиция (position + 1)
        generate_recursive(current_guess, position + 1, max_length, target_hash);
    }
}

int main() 
{
	/*char target_hash[3];
	scanf("%s", target_hash);*/
	char buffer[4]; // За пароли до 8 символа + '\0'
	generate_recursive(buffer, 0, 3, "abc"); // Пробва до 4 символа за теста!
}

