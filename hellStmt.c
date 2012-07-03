#include <stdio.h>
#include <assert.h>

#include "hellStmt.h"
#include "hellParser.h"
#include "hellLexer.h"

#include <stdlib.h>

static hellStmt *
allocateStmt(void)
{
    hellStmt *b = malloc(sizeof(hellStmt));

    if (b == NULL)
        return NULL;

    b->type = hellUndef;

    return b;
}

hellStmt *
createNum(unsigned long value)
{
    hellStmt *b = allocateStmt();

    printf("%s %lu\n", __func__, value);

    if (b == NULL)
        return NULL;

    b->type = hellNum;
    b->num = value;

    return b;
}

hellStmt *
createStr(char *value)
{
    hellStmt *b = allocateStmt();

    printf("%s '%s'\n", __func__, value);

    if (b == NULL)
        return NULL;

    b->type = hellStr;
    strncpy(b->str, value, sizeof(b->str));

    return b;
}

hellStmt *
createArg(hellStmt *arg)
{
    hellStmt *b = allocateStmt();

    printf("%s\n", __func__);

    if (b == NULL)
        return NULL;

    b->type = hellArg;

    return b;
}

hellStmt *
createAssign(hellStmt *func, hellStmt *arg)
{
    hellStmt *b = allocateStmt();

    printf("%s\n", __func__);

    if (b == NULL)
        return NULL;

    b->type = hellAssign;

    return b;
}

hellStmt *
createCall(hellStmt *lval, hellStmt *arg)
{
    hellStmt *b = allocateStmt();

    printf("%s\n", __func__);

    if (b == NULL)
        return NULL;

    b->type = hellCall;
    /* strcpy(b->str, value); */

    return b;
}

void deleteStmt(hellStmt *b)
{
    if (b == NULL)
        return;

    free(b);
}

int yyparse(hellStmt **expression, yyscan_t scanner);

hellStmt *
getAST(const char *expr)
{
    hellStmt *stmt;
    yyscan_t scanner;
    YY_BUFFER_STATE state;

    printf("stmt: %s\n", expr);

    if (yylex_init(&scanner))
        return NULL;

    //yyset_debug(100, scanner);

    state = yy_scan_string(expr, scanner);

    if (yyparse(&stmt, scanner))
        return NULL;

    yy_delete_buffer(state, scanner);

    yylex_destroy(scanner);

    return stmt;
}

void
evaluate(hellStmt *e)
{
    if (!e)
        return;

    switch (e->type) {
        case hellNum:
            printf("%s num %lu\n", __func__, e->num);
            break;
        case hellStr:
            printf("%s str %s\n", __func__, e->str);
            break;
        case hellCall:
            printf("%s call\n", __func__);
            break;
        case hellAssign:
            printf("%s assign\n", __func__);
            break;
        case hellArg:
            printf("%s arg\n", __func__);
            break;
        default:
            printf("%s %u\n", __func__, e->type);
    }
}

int main(void)
{
    hellStmt *e;
    char *test[] = {
        "call   \"c\"  , 3  ",
        "call\"c\"  , 3  ",
        "a=   \"c\"    ",
        "a   =   \"c\"    ",
        "a   =\"c\"    ",
        "call   \"c\"  , 3  ;call\"c\"  , 3  ;  a=   \"c\"    ",
        "a   =0x8    ",
        "a   = 0x10    ",
        "a=0xA;    ",
    };
    int i;

    for (i=0;i<9;++i) {
        printf("===============\n");
        e = getAST(test[i]);
        evaluate(e);
        deleteStmt(e);
    }

    return 0;
}
