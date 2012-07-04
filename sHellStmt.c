#include <string.h>
#include <stdlib.h>

#include "sHellStmt.h"
#include "sHellParser.h"
#include "sHellLexer.h"

static sHellAssignCbk   sHellAssign;
static sHellCallCbk     sHellCall;

static sHellStmt *sHellHeapAllocStmt(void);
static void sHellHeapFreeStmt(sHellStmt *stmt);
static char *sHellHeapAllocStr(unsigned int size);
static void sHellHeapFreeStr(char *str);

static sHellAllocStmtCbk    sHellAllocStmt = sHellHeapAllocStmt;
static sHellFreeStmtCbk     sHellFreeStmt  = sHellHeapFreeStmt;
static sHellAllocStrCbk     sHellAllocStr  = sHellHeapAllocStr;
static sHellFreeStrCbk      sHellFreeStr   = sHellHeapFreeStr;

sHellAllocStmtCbk
sHellSetStmtAllocCbk(sHellAllocStmtCbk cbk)
{
    sHellAllocStmtCbk old = sHellAllocStmt;

    sHellAllocStmt = cbk;

    return old;
}

sHellFreeStmtCbk
sHellSetStmtFreeCbk(sHellFreeStmtCbk cbk)
{
    sHellFreeStmtCbk old = sHellFreeStmt;

    sHellFreeStmt = cbk;

    return old;
}

sHellAllocStrCbk
sHellSetStrAllocCbk(sHellAllocStrCbk cbk)
{
    sHellAllocStrCbk old = sHellAllocStr;

    sHellAllocStr = cbk;

    return old;
}

sHellFreeStrCbk
sHellSetStrFreeCbk(sHellFreeStrCbk cbk)
{
    sHellFreeStrCbk old = sHellFreeStr;

    sHellFreeStr = cbk;

    return old;
}

static sHellStmt *
sHellHeapAllocStmt(void)
{
    return malloc(sizeof(sHellStmt));
}

static void
sHellHeapFreeStmt(sHellStmt *stmt)
{
    free(stmt);
}

static char *
sHellHeapAllocStr(unsigned int size)
{
    return malloc(size);
}

static void
sHellHeapFreeStr(char *str)
{
    free(str);
}

static sHellStmt *
sHellAllocateStmt(void)
{
    sHellStmt *b = sHellAllocStmt();

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
            sHellHeapFreeStr(t->str);
        sHellFreeStmt(t);
    }

    if (sHellStr == b->type && b->str)
        sHellHeapFreeStr(b->str);
    sHellFreeStmt(b);
}

sHellAssignCbk
sHellSetAssignCbk(sHellAssignCbk cbk)
{
    sHellAssignCbk old = sHellAssign;

    sHellAssign = cbk;

    return old;
}

sHellCallCbk
sHellSetCallCbk(sHellCallCbk cbk)
{
    sHellCallCbk old = sHellCall;

    sHellCall = cbk;

    return old;
}

char *
sHellProcessString(const char *str)
{
    size_t l = strlen(str) + 1;
    char *s = sHellHeapAllocStr(l);

    if (s == NULL)
        return s;

    return memcpy(s, str, l);
}

char *
sHellProcessQString(const char *str)
{
    size_t l = strlen(str) - 1;
    char *s = sHellHeapAllocStr(l);

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
