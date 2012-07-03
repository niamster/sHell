%{

/*
 * hellLexer.l file
 * To generate the lexical analyzer run: "flex hellLexer.l"
 */

#include "hellStmt.h"
#include "hellParser.h"

#include <stdio.h>

%}

%option outfile="hellLexer.c" header-file="hellLexer.h"

%option warn nodefault
%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

COMA        ","
SEMI        ";"
ASSIGN      "="

NUM         [0-9]+
STR         [a-zA-Z]+
HEX         0x[0-9a-zA-Z]+
QSTR        \"[ 0-9a-zA-Z]+\"
WS          [ \r\n\t]*

%%

{NUM}           { yylval->num = strtoul(yytext, (char **)NULL, 0); return TOKEN_NUM; }
{HEX}           { yylval->num = strtoul(yytext, (char **)NULL, 0); return TOKEN_NUM; }
{STR}           { strncpy(yylval->str, yytext, sizeof(yylval->str)); return TOKEN_STR; }
{QSTR}          { strncpy(yylval->str, yytext, sizeof(yylval->str)); return TOKEN_QSTR; }

{WS}            { }

{COMA}          { return TOKEN_COMA; }
{SEMI}          { return TOKEN_SEMI; }
{ASSIGN}        { return TOKEN_ASSIGN; }
.               { }

%%

int yyerror(hellStmt *e, yyscan_t scanner, const char *msg) {
    fprintf(stderr,"Error: '%s'\n" ,msg); return 0;
}
