#include "sHellStmt.h"
#include "sHellParser.h"
#include "sHellLexer.h"

static unsigned int sHellOrphanedStmts = 0;

static sHellAssignCbk   sHellAssign;
static sHellCallCbk     sHellCall;

static sHellStmt *
sHellAllocateStmt(void)
{
    sHellStmt *b = malloc(sizeof(sHellStmt));

    if (b == NULL)
        return NULL;

    b->type = sHellUndef;
    b->next = NULL;

    ++sHellOrphanedStmts;

    return b;
}

static
void
sHellDeleteStmt(sHellStmt *b)
{
    sHellStmt *n, *t;

    if (b == NULL)
        return;

    n = b->next;
    while (n) {
        t = n;
        n = n->next;
        free(t);

        --sHellOrphanedStmts;
    }

    free(b);

    --sHellOrphanedStmts;
}

sHellAssignCbk
sHellSetAssignCbk(sHellAssignCbk cbk)
{
    sHellAssign = cbk;
}

sHellCallCbk
sHellSetCallCbk(sHellCallCbk cbk)
{
    sHellCall = cbk;
}

sHellStmt *
sHellCreateNum(unsigned long value)
{
    sHellStmt *b = sHellAllocateStmt();

    if (b == NULL)
        return NULL;

    b->type = sHellNum;
    b->num = value;

    return b;
}

sHellStmt *
sHellCreateStr(char *value)
{
    sHellStmt *b = sHellAllocateStmt();

    if (b == NULL)
        return NULL;

    b->type = sHellStr;
    strncpy(b->str, value, sizeof(b->str));

    return b;
}

sHellStmt *
sHellAppendArg(sHellStmt *args, sHellStmt *arg)
{
    sHellStmt *a = args;

    while (a->next)
        a = a->next;
    a->next = arg;

    return args;
}

void
sHellPerformAssign(sHellStmt *lval, sHellStmt *arg)
{
    sHellAssign(lval->str, arg);

    sHellDeleteStmt(lval);
    sHellDeleteStmt(arg);
}

void
sHellPerformCall(sHellStmt *call, sHellStmt *args)
{
    sHellCall(call->str, args);

    sHellDeleteStmt(call);
    sHellDeleteStmt(args);
}

void
sHellEvaluate(sHellStmt *e)
{
    if (!e)
        return;

    switch (e->type) {
        case sHellNum:
            printf("%s num %lu\n", __func__, e->num);
            break;
        case sHellStr:
            printf("%s str '%s'\n", __func__, e->str);
            break;
        default:
            printf("%s %u\n", __func__, e->type);
    }

    sHellDeleteStmt(e);

    if (sHellOrphanedStmts)
        printf("WARNING: %u orphaned statements\n", sHellOrphanedStmts);
}

int yyparse(sHellStmt **expression, yyscan_t scanner);

void
sHellParse(const char *expr)
{
    sHellStmt *stmt;
    yyscan_t scanner;
    YY_BUFFER_STATE state;
    extern int yydebug;

    if (yylex_init(&scanner))
        return;

    /* yyset_debug(1, scanner); */
    /* yydebug = 1; */

    state = yy_scan_string(expr, scanner);

    yyparse(&stmt, scanner);

    yy_delete_buffer(state, scanner);

    yylex_destroy(scanner);
}
