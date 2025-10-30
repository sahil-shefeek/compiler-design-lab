%{
	#include <stdio.h>

	int yylex(void);
	void yyerror(const char *);

%}

%token IDENTIFIER

%%

program:
       line
       | program line
       ;

line:
    IDENTIFIER '\n' { printf("Valid identifier\n"); }
    | '\n' {}
    | error '\n' { yyerrok; }
    ;

%%

void yyerror(const char *s)
{
	fprintf(stderr, "Error: %s\n", s);
}

int main()
{
	yyparse();
	return 0;
}
