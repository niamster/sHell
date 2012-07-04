#include <string.h>
#include <stdlib.h>

#include "sHellStmt.h"
#include "sHellParser.h"
#include "sHellLexer.h"

static sHellAssignCbk   sHellAssign;
static sHellCallCbk     sHellCall;

static sHellStmt *
sHellAllocateStmt(void)
{
    sHellStmt *b = malloc(sizeof(sHellStmt));

    if (b == NULL)
        return NULL;

    b->type = sHellUndef;
    b->str  = NULL;
    b->next = NULL;

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
        if (sHellStr == t->type && t->str)
            free(t->str);
        free(t);
    }

    if (sHellStr == b->type && b->str)
        free(b->str);
    free(b);
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

char *
sHellProcessString(const char *str)
{
    size_t l = strlen(str) + 1;
    char *s = malloc(l);

    if (s == NULL)
        return s;

    return memcpy(s, str, l);
}

char *
sHellProcessQString(const char *str)
{
    size_t l = strlen(str) - 1;
    char *s = malloc(l);

    if (s == NULL)
        return s;

    memcpy(s, str+1, l);

    s[l-1] = '\0';

    return s;
}

unsigned long
sHellProcessNum(const char *str)
{
    return strtoul(str, (char **)NULL, 0);
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
    b->str = value;

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
