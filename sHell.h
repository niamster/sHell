#ifndef _SHELL_H_
#define _SHELL_H_

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

void sHellParse(const char *expr);

typedef void (*sHellAssignCbk)(const char *name, const sHellStmt *arg);
typedef void (*sHellCallCbk)(const char *name, const sHellStmt *args);

sHellAssignCbk sHellSetAssignCbk(sHellAssignCbk cbk);
sHellCallCbk sHellSetCallCbk(sHellCallCbk cbk);

#endif
