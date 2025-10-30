%{
#include <stdio.h>
%}

%%
^(\+91[\- ]?|91[\- ]?)?[6-9][0-9]{9}$      { printf("Valid phone number: %s\n", yytext); }

^[ \t\n]*$                                   {  }

^.*$                                         { printf("Invalid phone number: %s\n", yytext); }
%%

int yywrap() {
    return 1;

}
int main() {
    printf("Enter a phone number: ");
    yylex();
    return 0;
}
