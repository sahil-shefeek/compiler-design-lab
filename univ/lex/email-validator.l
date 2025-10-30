
%{
#include <stdio.h>
%}

%%
[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}   { printf("Valid email address\n"); }
.+                                                 { printf("Invalid email address\n"); }
%%

int main() {
    printf("Enter an email address: ");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
