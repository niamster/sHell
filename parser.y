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
%type <stmt> call
%type <stmt> lval
%type <stmt> args
%type <stmt> arg

%%

input
    : expr                                  { *stmt = $1; }
    ;

expr
    : call args                             { $$ = createCall( $1, $2 ); }
    | lval TOKEN_ASSIGN arg                 { $$ = createAssign( $1, $3 ); }
    | expr TOKEN_SEMI                       { }
    | expr TOKEN_SEMI expr                  { }
    ;

call
    : TOKEN_STR                             { $$ = createStr($1); }
    ;

lval
    : TOKEN_STR                             { $$ = createStr($1); }
    ;

args
    : arg                                   { $$ = createArg($1); }
    | arg TOKEN_COMA arg                    { }
    ;

arg
    : TOKEN_NUM                             { $$ = createNum($1); }
    | TOKEN_QSTR                            { $$ = createStr($1); }
    ;

%%
