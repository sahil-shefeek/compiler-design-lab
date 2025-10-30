%{
	#include <stdio.h>
	
	int STATE = 0;
	int first_number = 0;
	char op;
%}

number [0-9]+
operator [+\-*/]

%%

{number} { 
	if (STATE == 0) {
		first_number = atoi(yytext);
		printf("Enter operator ( +, - , * , / ): ");
		STATE = 1;
	} else if (STATE == 2) {
		int second_number = atoi(yytext);
		int result;
		switch (op) {
			case '+':
				result = first_number + second_number;
				break;
		}
		printf("%d %c %d = %d", first_number, op, second_number, result);
		STATE = 0;
	} else {
		printf("Invalid input. Resetting...\n");
		STATE = 0;
	}
}

{operator} { 
	op = yytext[0];
	printf("Enter second operand: ");
	STATE = 2;
}


%%

int yywrap()
{
	return 1;
}

int main()
{
	printf("Enter first operand: ");
	yylex();
	return 0;
}


