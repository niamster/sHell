%{

/*
 * sHellParser.y file
 * To generate the parser run: "bison sHellParser.y"
 */

#include "sHellStmt.h"
#include "sHellParser.h"
#include "sHellLexer.h"

int yyerror(yyscan_t scanner, sHellStmt **expression, const char *msg);

%}

%output  "sHellParser.c"
%defines "sHellParser.h"

%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { sHellStmt **stmt }
%parse-param { yyscan_t scanner }

%union {
    unsigned long num;
    char str[SHELL_MAX_STR_LEN];
    sHellStmt *stmt;
}

%token TOKEN_COMA
%token TOKEN_SEMI
%token TOKEN_ASSIGN
%token TOKEN_WS

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
    : exprs                                  { *stmt = $1; }
    ;

exprs
    : expr                                  { }
    | exprs TOKEN_SEMI                      { }
    | exprs TOKEN_SEMI expr                 { }
    ;

expr
    : call args                             { $$ = sHellPerformCall($1, $2); }
    | call                                  { $$ = sHellPerformCall($1, NULL); }
    | lval TOKEN_ASSIGN arg                 { $$ = sHellPerformAssign($1, $3); }
    ;

call
    : TOKEN_STR                             { $$ = sHellCreateStr($1); }
    ;

lval
    : TOKEN_STR                             { $$ = sHellCreateStr($1); }
    ;

args
    : arg                                   { $$ = sHellAppendArg($1, NULL); }
    | arg TOKEN_COMA arg                    { $$ = sHellAppendArg($1, $3); }
    ;

arg
    : TOKEN_NUM                             { $$ = sHellCreateNum($1); }
    | TOKEN_QSTR                            { $$ = sHellCreateStr($1); }
    ;

%%
