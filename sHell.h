#ifndef _SHELL_H_
#define _SHELL_H_

typedef enum sHellTokenType {
    sHellNum,
    sHellStr,

    sHellUndef,
} sHellTokenType;

struct sHellStmt;

typedef struct sHellStmt {
    sHellTokenType type;

    union {
        unsigned long   num;
        char            *str;
    };

    struct sHellStmt *next;
} sHellStmt;

void sHellParse(const char *expr);

typedef void (*sHellAssignCbk)(const char *name, const sHellStmt *arg);
typedef void (*sHellCallCbk)(const char *name, const sHellStmt *args);

sHellAssignCbk sHellSetAssignCbk(sHellAssignCbk cbk);
sHellCallCbk sHellSetCallCbk(sHellCallCbk cbk);

typedef sHellStmt *(*sHellAllocStmtCbk)(void);
typedef void (*sHellFreeStmtCbk)(sHellStmt *stmt);
typedef char *(*sHellAllocStrCbk)(unsigned int size);
typedef void (*sHellFreeStrCbk)(char *str);

sHellAllocStmtCbk sHellSetStmtAllocCbk(sHellAllocStmtCbk cbk);
sHellFreeStmtCbk sHellSetStmtFreeCbk(sHellFreeStmtCbk cbk);
sHellAllocStrCbk sHellSetStrAllocCbk(sHellAllocStrCbk cbk);
sHellFreeStrCbk sHellSetStrFreeCbk(sHellFreeStrCbk cbk);

#endif
