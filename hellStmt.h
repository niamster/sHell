#ifndef _HELLSTMT_H_
#define _HELLSTMT_H_

#define HELL_MAX_STR_LEN 128

typedef enum hellTokenType {
    hellNum,
    hellStr,

    hellCall,
    hellAssign,

    hellArg,

    hellUndef,
} hellTokenType;

typedef struct hellCallArg {
    union {
        unsigned long num;
        char str[HELL_MAX_STR_LEN];
    };
} hellCallArg;

typedef struct hellCallArgs {
    hellCallArg arg;
    hellCallArg *next;
} hellCallArgs;

typedef struct hellStmt {
    hellTokenType type;

    union {
        unsigned long   num;
        char            str[HELL_MAX_STR_LEN];
        hellCallArg     arg;
        hellCallArgs    args;
    };
} hellStmt;

hellStmt *createNum(unsigned long value);
hellStmt *createStr(char *value);

hellStmt *createArg(hellStmt *arg);

hellStmt *createCall(hellStmt *func, hellStmt *args);
hellStmt *createAssign(hellStmt *lval, hellStmt *arg);

void deleteStmt(hellStmt *b);

#endif
