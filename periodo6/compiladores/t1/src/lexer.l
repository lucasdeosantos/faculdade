%{
#include <stdio.h>
#include <string.h>
#include "parser.tab.h"

int yylval;
%}

digito      [0-9]
letra       [a-zA-Z]
ID          {letra}({letra}|{digito})*
NUM         {digito}+(\.{digito}+)?([Ee][+-]?{digito}+)?

%%

"program"               { return PROGRAM; }
"var"                   { return VAR; }
"function"              { return FUNCTION; }
"procedure"             { return PROCEDURE; }
"begin"                 { return BEGIN_TOKEN; }
"end"                   { return END; }
"if"                    { return IF; }
"then"                  { return THEN; }
"else"                  { return ELSE; }
"while"                 { return WHILE; }
"do"                    { return DO; }
"or"                    { return OR; }
"integer"               { return INTEIRO; }
"real"                  { return REAL; }

":="                    { return OPERADOR_ATRIBUICAO; }

"="                     { return OPERADOR_RELACIONAL; }
"!="                    { return OPERADOR_RELACIONAL; }
"<"                     { return OPERADOR_RELACIONAL; }
"<="                    { return OPERADOR_RELACIONAL; }
">"                     { return OPERADOR_RELACIONAL; }
">="                    { return OPERADOR_RELACIONAL; }

"+"                     { return MAIS; }
"-"                     { return MENOS; }

"*"                     { return OPERADOR_MULTIPLICATIVO; }
"/"                     { return OPERADOR_MULTIPLICATIVO; }
"div"                   { return OPERADOR_MULTIPLICATIVO; }
"mod"                   { return OPERADOR_MULTIPLICATIVO; }
"and"                   { return OPERADOR_MULTIPLICATIVO; }

"("                     { return ABRE_PARENTESES; }
")"                     { return FECHA_PARENTESES; }
";"                     { return PONTO_VIRGULA; }
","                     { return VIRGULA; }
":"                     { return DOIS_PONTOS; }
"."                     { return PONTO_FINAL; }

{digit}+                { yylval = atoi(yytext); return NUM; }
{id}                    { yylval = strdup(yytext); return ID; }

[ \t\r\n]+              { /* ignora espaços, tabs e quebras de linha */ }
"//".*                  { /* ignora comentários de linha */ }
.                       { printf("Caractere inesperado: %s\n", yytext); return yytext[0]; }

%%

int yywrap() {
    return 1;
}