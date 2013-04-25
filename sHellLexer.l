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
%option noyyalloc noyyrealloc noyyfree
%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

COMA        ","
SEMI        ";"
ASSIGN      "="

NUM         [0-9]+
STR         [_0-9a-zA-Z]+
HEX         0x[0-9a-zA-Z]+
QSTR        \"[^\"]+\"
WS          [ \r\n\t]*
COMMENT     #.*

%%

{NUM}           { yylval->num = sHellProcessNum(yytext); return TOKEN_NUM; }
{HEX}           { yylval->num = sHellProcessNum(yytext); return TOKEN_NUM; }
{STR}           { yylval->str = sHellProcessString(yytext); return TOKEN_STR; }
{QSTR}          { yylval->str = sHellProcessQString(yytext); return TOKEN_QSTR; }

{WS}            { }

{COMA}          { return TOKEN_COMA; }
{SEMI}          { return TOKEN_SEMI; }
{ASSIGN}        { return TOKEN_ASSIGN; }
{COMMENT}       { return TOKEN_COMMENT; }
.               { }

%%

void *yyalloc(size_t sz, void *extra) {
  return sHellAlloc(sz);
}

void* yyrealloc(void *p, size_t sz, void *extra) {
  if (p)
    sHellFree(p);

  if (sz)
    return sHellAlloc(sz);
  else
    return NULL;
}

void yyfree(void *p, void *extra) {
  sHellFree(p);
}

int yyerror(sHellStmt *e, yyscan_t scanner, const char *msg) {
    fprintf(stderr,"Error: '%s'\n" ,msg); return 0;
}
