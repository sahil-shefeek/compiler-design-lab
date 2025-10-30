%{
	#include <stdio.h>
	int yylex(void);
	void yyerror(const char *);
%}

%token NUMBER

%left '+' '-'
%left '*' '/'
%right '^'

%%

line:
    expr '\n' { printf("Valid expression\n"); }
    | '\n' {}
    ;

expr:
    expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '^' expr
    | '(' expr ')'
    | NUMBER
    ;

%%

void yyerror(const char *s) {
	fprintf(stderr, "Error: %s", s);
}

int main()
{
	printf("Enter an expression to parse: ");
	yyparse();
	return 0;
}
