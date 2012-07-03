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

hellStmt *createNum(unsigned long value);
hellStmt *createStr(char *value);

hellStmt *appendArg(hellStmt *args, hellStmt *arg);

hellStmt *performCall(hellStmt *func, hellStmt *args);
hellStmt *performAssign(hellStmt *lval, hellStmt *arg);

void deleteStmt(hellStmt *b);

#endif
