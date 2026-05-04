#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXN 1000000

int main()
{
    int start = 0;
    int count = 0;

    printf("Enter start position: ");
    scanf("%d", &start);

    int numbers[MAXN];

    for (int i = 0; i < MAXN; i++) {
        numbers[i] = i;
    }
    



    /*
    Condition if number is simple
    
    printf("%d\n", i);
    count++;
    
    */



    printf("\nTotal simple numbers: %d\n", count);

    return EXIT_SUCCESS;
}