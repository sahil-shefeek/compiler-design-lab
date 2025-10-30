%{
	#include <stdio.h>
	
	void is_prime(int number) {
		if (number < 2) {
			printf("%d is not a prime!\n", number);
			return;
		}
		for (int i = 2; i < number / 2 + 1; i++) {
			if (number % i == 0) {
				printf("%d is not a prime!\n", number);
				return;
			}
		}
		printf("%d is a prime!\n", number);
	}
%}

number [0-9]+

%%

{number} { is_prime(atoi(yytext)); }

%%

int yywrap()
{
	return 1;
}

int main()
{
	printf("Enter a number: ");
	yylex();
	return 0;
}
