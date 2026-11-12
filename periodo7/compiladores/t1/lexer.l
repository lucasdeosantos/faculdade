%{
#include "ast.h"
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>
%}

DIGITO      [0-9]
LETRA       [a-zA-Z]
ID          ({LETRA}|_)+({LETRA}|{DIGITO}|_)*
NUM         {DIGITO}+(\.{DIGITO}+)?([eE][+-]?{DIGITO}+)?

%%

"program"               { return PROGRAM; }
"var"                   { return VAR; }
"integer"               { return INTEIRO; }
"real"                  { return REAL; }
"function"              { return FUNCTION; }
"procedure"             { return PROCEDURE; }
"begin"                 { return BEGIN_TOKEN; }
"end"                   { return END; }
"if"                    { return IF; }
"then"                  { return THEN; }
"else"                  { return ELSE; }
"while"                 { return WHILE; }
"do"                    { return DO; }
"read"                  { return READ; }
"write"                 { return WRITE; }
"writeln"               { return WRITELN; }

":="                    { return ATRIBUICAO; }

"="                     { return EQ; }
"<>"                    { return NE; }
"<="                    { return LE; }
"<"                     { return LT; }
">="                    { return GE; }
">"                     { return GT; }

"+"                     { return MAIS; }
"-"                     { return MENOS; }
"or"                    { return OR; }

"*"                     { return MULTIPLICACAO; }
"/"                     { return DIVISAO; }
"div"                   { return DIV; }
"mod"                   { return MOD; }
"and"                   { return AND; }

"."                     { return PONTO_FINAL; }
":"                     { return DOIS_PONTOS; }
";"                     { return PONTO_VIRGULA; }
","                     { return VIRGULA; }
"("                     { return ABRE_PARENTESES; }
")"                     { return FECHA_PARENTESES; }

{ID}                    { yylval.id = strdup(yytext); return ID; }
{NUM}                   { yylval.num = atof(yytext); return NUM; }

[ \t\n\r]+              { /* Ignorar espaços em branco */ }
.                       { printf("Caractere não reconhecido: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}