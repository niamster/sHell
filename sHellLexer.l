%{

/*
 * sHellLexer.l file
 * To generate the lexical analyzer run: "flex sHellLexer.l"
 */

#include "sHellStmt.h"
#include "sHellParser.h"

%}

%option outfile="sHellLexer.c" header-file="sHellLexer.h"

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

{NUM}           { yylval->num = sHellProcessNum(yytext); return TOKEN_NUM; }
{HEX}           { yylval->num = sHellProcessNum(yytext); return TOKEN_NUM; }
{STR}           { yylval->str = sHellProcessString(yytext); return TOKEN_STR; }
{QSTR}          { yylval->str = sHellProcessQString(yytext); return TOKEN_QSTR; }

{WS}            { }

{COMA}          { return TOKEN_COMA; }
{SEMI}          { return TOKEN_SEMI; }
{ASSIGN}        { return TOKEN_ASSIGN; }
.               { }

%%

int yyerror(sHellStmt *e, yyscan_t scanner, const char *msg) {
    fprintf(stderr,"Error: '%s'\n" ,msg); return 0;
}
