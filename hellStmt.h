#ifndef _HELLSTMT_H_
#define _HELLSTMT_H_

#define HELL_MAX_STR_LEN 128

typedef enum hellTokenType {
    hellNum,
    hellStr,

    hellUndef,
} hellTokenType;

struct hellStmt;

typedef struct hellStmt {
    hellTokenType type;

    union {
        unsigned long   num;
        char            str[HELL_MAX_STR_LEN];
    };
    struct hellStmt *next;
} hellStmt;

hellStmt *hellCreateNum(unsigned long value);
hellStmt *hellCreateStr(char *value);

hellStmt *hellAppendArg(hellStmt *args, hellStmt *arg);

hellStmt *hellPerformCall(hellStmt *func, hellStmt *args);
hellStmt *hellPerformAssign(hellStmt *lval, hellStmt *arg);

hellStmt *hellParse(const char *expr);

void hellDeleteStmt(hellStmt *b);

#endif
