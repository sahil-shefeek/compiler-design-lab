%{
	#include <stdio.h>

	int vowel_count = 0;
	int consonant_count = 0;
%}

%%

[aeiouAEIOU] {vowel_count++;}
[a-zA-Z] {consonant_count++;}
\n              { /* Do nothing */ }
. {}

%%

int yywrap() {
	return 1;
}

int main() {
	printf("Enter some text: ");

	yylex();

	printf("Vowel count: %d\n", vowel_count);
	printf("Consonant count: %d\n", consonant_count);

	return 0;
}
