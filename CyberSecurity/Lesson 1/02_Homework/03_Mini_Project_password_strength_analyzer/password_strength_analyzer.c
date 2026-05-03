#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *weak[] = {"123456","password","admin"};

//password[i]

int analyze_password(const char *password)
{
	int points = 0;
	int lowerCase = 0;
	int upperCase = 0;
	int digit = 0;
	int symbols = 0;
	
	for (int i = 0; i < strlen(password);i++)
	{
		points++;
		if (islower(password[i])) { lowerCase++; }
		else if (isupper(password[i])) { upperCase++; }
		else if (isdigit(password[i])) { digit++; }
		else { symbols++; }
	}
	
	printf("\nPassword: %s\n",password);
	//printf("\n-- Character analyzer --\n");
	printf("Length: %d\n", strlen(password));
	printf("LowerCase: %d\n", lowerCase);
	printf("UpperCase: %d\n", upperCase);
	printf("Digits: %d\n", digit);
	printf("Symbols: %d\n\n", symbols);
	if (lowerCase > 0) { points += 10; }
	if (upperCase > 0) { points += 10; }
	if (digit > 0) { points += 15; }
	if (symbols > 0) { points += 20; }
	printf("Total points: %d\n", points);
	
	
	for (int i = 0; i < (sizeof(weak)/sizeof(weak[0])); i++)
	{
		if ((strcmp(password,weak[i])) == 0)
		{
			printf("Weak password detected! Penalty: -20 points!\n");
			points -= 20;
		}
	}
	
	if (points < 0) { points = 0; }
	if (points > 100) {points = 100; }
	printf("\n");
	return points;
}

int main()
{
	char password[100] = "";
	printf("Enter password: ");
	scanf("%s",password);

	printf("Score [0-100]: %d\n",analyze_password(password));
	
	printf("\nPassword analyze of key used in Vigener");
	printf("Score [0-100]: %d",analyze_password("LEMON"));

    return EXIT_SUCCESS;
}
