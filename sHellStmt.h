#ifndef _SHELLSTMT_H_
#define _SHELLSTMT_H_

#include "sHell.h"

sHellStmt *sHellCreateNum(unsigned long value);
sHellStmt *sHellCreateStr(char *value);

sHellStmt *sHellAppendArg(sHellStmt *args, sHellStmt *arg);

sHellStmt *sHellPerformCall(sHellStmt *func, sHellStmt *args, int res);
void sHellPerformAssign(sHellStmt *lval, sHellStmt *arg);

char *sHellProcessString(const char *str);
char *sHellProcessQString(const char *str);
unsigned long sHellProcessNum(const char *str);

#endif
