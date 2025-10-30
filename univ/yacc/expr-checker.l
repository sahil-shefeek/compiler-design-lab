%{
	#include "y.tab.h"
%}

number [0-9]+

%%

{number} { return NUMBER; }

[ \t] ;

[\n] { return '\n'; }

. { return yytext[0]; }

%%

int yywrap()
{
	return 1;
}
