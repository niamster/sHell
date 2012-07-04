%{

/*
 * hellParser.y file
 * To generate the parser run: "bison hellParser.y"
 */

#include "hellStmt.h"
#include "hellParser.h"
#include "hellLexer.h"

int yyerror(yyscan_t scanner, hellStmt **expression, const char *msg);

%}

%output  "hellParser.c"
%defines "hellParser.h"

%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { hellStmt **stmt }
%parse-param { yyscan_t scanner }

%union {
    unsigned long num;
    char str[HELL_MAX_STR_LEN];
    hellStmt *stmt;
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
    : call args                             { $$ = hellPerformCall($1, $2); }
    | lval TOKEN_ASSIGN arg                 { $$ = hellPerformAssign($1, $3); }
    ;

call
    : TOKEN_STR                             { $$ = hellCreateStr($1); }
    ;

lval
    : TOKEN_STR                             { $$ = hellCreateStr($1); }
    ;

args
    : arg                                   { $$ = hellAppendArg($1, NULL); }
    | arg TOKEN_COMA arg                    { $$ = hellAppendArg($1, $3); }
    ;

arg
    : TOKEN_NUM                             { $$ = hellCreateNum($1); }
    | TOKEN_QSTR                            { $$ = hellCreateStr($1); }
    ;

%%
