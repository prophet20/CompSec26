#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // --- ЧАСТ 1: АЛИСА (Симулация на жертвата) ---
    // Алиса стартира програмата в момент 'T'.
    time_t alice_seed = time(NULL); 
    srand(alice_seed); 
    
    int secret_token = rand(); // Алиса генерира това число
    
    printf("Alice: My secret token is %d\n", secret_token);
    printf("Alice: (I generated this at timestamp %ld)\n", alice_seed);
    
    printf("Alice 2: %d\n", rand());
    printf("Alice 3: %d\n", rand());
    printf("Alice 4: %d\n", rand());
    
    // --- ЧАСТ 2: ЕВА (Вашата задача) ---
    printf("\n--- Eve: Starting Brute-Force Attack ---\n");
    
    // Ева взима текущото време. Тя знае, че Алиса е генерирала токена
    // малко преди това (напр. в интервала [сега-600 ... сега]).
    time_t now = time(NULL);
    
    int found = 0;
    
    // TODO: Напишете цикъл, който проверява всяка секунда назад
    for (time_t t = now - 600; t <= now; t++) {
        // 1. Сложете текущото 't' като seed: srand(t);
        // 2. Генерирайте число: int guess = rand();
        // 3. Проверете дали guess == secret_token
        // 4. Ако съвпадат - честито! Принтирайте намерения Seed и break;
        
        srand(t);
        if (rand() == secret_token) {
            printf("[CRACKED!] The seed was: %ld\n", t);
            printf("Crack 2: %d\n", rand());
            printf("Crack 3: %d\n", rand());
            printf("Crack 4: %d\n", rand());
            found = 1;
            break;
        }
    }
    
    if (!found) printf("Eve: Failed to crack the seed.\n");
    return EXIT_SUCCESS;
}