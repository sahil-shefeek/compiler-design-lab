%{
	#include "y.tab.h"
%}

letter [a-zA-Z]
digit [0-9]

%%

[a-zA-Z_][a-zA-Z0-9_]*   { return IDENTIFIER; }
[\n] { return '\n'; }
[ \t] ;
. { return yytext[0]; }

%%

int yywrap()
{
	return 1;
}
