#ifndef _SHELLSTMT_H_
#define _SHELLSTMT_H_

#define SHELL_MAX_STR_LEN 128

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
        char            str[SHELL_MAX_STR_LEN];
    };
    struct sHellStmt *next;
} sHellStmt;

sHellStmt *sHellCreateNum(unsigned long value);
sHellStmt *sHellCreateStr(char *value);

sHellStmt *sHellAppendArg(sHellStmt *args, sHellStmt *arg);

sHellStmt *sHellPerformCall(sHellStmt *func, sHellStmt *args);
sHellStmt *sHellPerformAssign(sHellStmt *lval, sHellStmt *arg);

sHellStmt *sHellParse(const char *expr);

void sHellDeleteStmt(sHellStmt *stmt);

#endif
