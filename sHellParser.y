%{

/*
 * sHellParser.y file
 * To generate the parser run: "bison sHellParser.y"
 */

#include "sHellStmt.h"
#include "sHellParser.h"
#include "sHellLexer.h"

#define YYMALLOC sHellAlloc
#define YYFREE sHellFree


int yyerror(yyscan_t scanner, sHellStmt **expression, const char *msg);

%}

%output  "sHellParser.c"
%defines "sHellParser.h"

%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { sHellStmt **stmt }
%parse-param { void *scanner }

%union {
    unsigned long num;
    char *str;
    sHellStmt *stmt;
}

%token TOKEN_COMA
%token TOKEN_SEMI
%token TOKEN_ASSIGN
%token TOKEN_WS
%token TOKEN_COMMENT

%token <num> TOKEN_NUM
%token <str> TOKEN_STR
%token <str> TOKEN_QSTR

%type <stmt> expr
%type <stmt> exprs
%type <stmt> call
%type <stmt> lval
%type <stmt> args
%type <stmt> arg

%%

input
    : exprs                                 { }
    ;

exprs
    : expr                                  { }
    | exprs TOKEN_SEMI                      { }
    | exprs TOKEN_SEMI expr                 { }
    | TOKEN_COMMENT                         { }
    | exprs TOKEN_COMMENT                   { }
    ;

expr
    : call args                             { sHellPerformCall($1, $2, 0); }
    | call                                  { sHellPerformCall($1, NULL, 0); }
    | lval TOKEN_ASSIGN arg                 { sHellPerformAssign($1, $3); }
    | lval TOKEN_ASSIGN call args           { sHellPerformAssign($1, sHellPerformCall($3, $4, 1)); }
    | lval TOKEN_ASSIGN call                { sHellPerformAssign($1, sHellPerformCall($3, NULL, 1)); }
    ;

call
    : TOKEN_STR                             { $$ = sHellCreateStr($1); }
    ;

lval
    : TOKEN_STR                             { $$ = sHellCreateStr($1); }
    ;

args
    : arg                                   { $$ = sHellAppendArg($1, NULL); }
    | args TOKEN_COMA arg                   { $$ = sHellAppendArg($1, $3); }
    ;

arg
    : TOKEN_NUM                             { $$ = sHellCreateNum($1); }
    | TOKEN_QSTR                            { $$ = sHellCreateStr($1); }
    ;

%%
