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
    b->next = NULL;

    return b;
}

hellStmt *
createNum(unsigned long value)
{
    hellStmt *b = allocateStmt();

    /* printf("%s %lu\n", __func__, value); */

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

    /* printf("%s '%s'\n", __func__, value); */

    if (b == NULL)
        return NULL;

    b->type = hellStr;
    strncpy(b->str, value, sizeof(b->str));

    return b;
}

hellStmt *
appendArg(hellStmt *args, hellStmt *arg)
{
    hellStmt *a = args;

    while (a->next)
        a = a->next;
    a->next = arg;

    return args;
}

hellStmt *
performAssign(hellStmt *lval, hellStmt *arg)
{
    printf("%s = ", lval->str);
    while (arg) {
        switch (arg->type) {
            case hellNum:
                printf("%u", arg->num);
                break;
            case hellStr:
                printf("'%s'", arg->str);
                break;
            default:
                printf("(undef)");
                break;
        }
        arg = arg->next;

        if (arg)
            printf(", ");
    }
    printf("\n");

    return NULL;
}

hellStmt *
performCall(hellStmt *call, hellStmt *arg)
{
    printf("%s(", call->str);
    while (arg) {
        switch (arg->type) {
            case hellNum:
                printf("%u", arg->num);
                break;
            case hellStr:
                printf("'%s'", arg->str);
                break;
            default:
                printf("(undef)");
                break;
        }
        arg = arg->next;

        if (arg)
            printf(", ");
    }
    printf(")\n");

    return NULL;
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
    extern int yydebug;

    printf("stmt: %s\n", expr);

    if (yylex_init(&scanner))
        return NULL;

    /* yyset_debug(100, scanner); */
    /* yydebug = 1; */

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
            printf("%s str '%s'\n", __func__, e->str);
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
        "call\"c\"    ",
        "a=   \"c\"    ",
        "a   =   \"c\"    ",
        "a   =\"c\"    ",
        "callA   \"c b\"  , 3  ;callB\"c\"  , 3  ;  a=   \"c\"    ",
        "a   =0x8    ",
        "a   = 0x10    ",
        "a=0xA;    b   = 0x10;; ; ;",
        "a=0;; ;",
        "a= \"   2\";; ;",
    };
    int i;

    for (i=0;i<sizeof(test)/sizeof(*test);++i) {
        printf("===============\n");
        e = getAST(test[i]);
        evaluate(e);
        deleteStmt(e);
    }

    return 0;
}
