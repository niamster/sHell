#include <stdio.h>
#include <assert.h>

#include "hellStmt.h"
#include "hellParser.h"
#include "hellLexer.h"

#include <stdlib.h>

static unsigned int hellOrphanedStmts = 0;

static hellStmt *
allocateStmt(void)
{
    hellStmt *b = malloc(sizeof(hellStmt));

    if (b == NULL)
        return NULL;

    b->type = hellUndef;
    b->next = NULL;

    ++hellOrphanedStmts;

    return b;
}

void hellDeleteStmt(hellStmt *b)
{
    hellStmt *n, *t;

    if (b == NULL)
        return;

    n = b->next;
    while (n) {
        t = n;
        n = n->next;
        free(t);

        --hellOrphanedStmts;
    }

    free(b);

    --hellOrphanedStmts;
}

hellStmt *
hellCreateNum(unsigned long value)
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
hellCreateStr(char *value)
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
hellAppendArg(hellStmt *args, hellStmt *arg)
{
    hellStmt *a = args;

    while (a->next)
        a = a->next;
    a->next = arg;

    return args;
}

hellStmt *
hellPerformAssign(hellStmt *lval, hellStmt *arg)
{
    hellStmt *a = arg;

    printf("%s = ", lval->str);
    while (a) {
        switch (a->type) {
            case hellNum:
                printf("%u", a->num);
                break;
            case hellStr:
                printf("'%s'", a->str);
                break;
            default:
                printf("(undef)");
                break;
        }
        a = a->next;

        if (a)
            printf(", ");
    }
    printf("\n");

    hellDeleteStmt(lval);
    hellDeleteStmt(arg);

    return NULL;
}

hellStmt *
hellPerformCall(hellStmt *call, hellStmt *arg)
{
    hellStmt *a = arg;

    printf("%s(", call->str);
    while (a) {
        switch (a->type) {
            case hellNum:
                printf("%u", a->num);
                break;
            case hellStr:
                printf("'%s'", a->str);
                break;
            default:
                printf("(undef)");
                break;
        }
        a = a->next;

        if (a)
            printf(", ");
    }
    printf(")\n");

    hellDeleteStmt(call);
    hellDeleteStmt(arg);

    return NULL;
}

void
hellEvaluate(hellStmt *e)
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

    hellDeleteStmt(e);

    if (hellOrphanedStmts)
        printf("WARNING: %u orphaned statements\n", hellOrphanedStmts);
}

int yyparse(hellStmt **expression, yyscan_t scanner);

hellStmt *
hellParse(const char *expr)
{
    hellStmt *stmt;
    yyscan_t scanner;
    YY_BUFFER_STATE state;
    extern int yydebug;

    printf("stmt: %s\n", expr);

    if (yylex_init(&scanner))
        return NULL;

    /* yyset_debug(1, scanner); */
    /* yydebug = 1; */

    state = yy_scan_string(expr, scanner);

    yyparse(&stmt, scanner);

    yy_delete_buffer(state, scanner);

    yylex_destroy(scanner);

    return stmt;
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
        e = hellParse(test[i]);
        hellEvaluate(e);
    }

    return 0;
}
