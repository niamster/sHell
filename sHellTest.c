#include <stdio.h>
#include <assert.h>

#include "sHellStmt.h"

int main(void)
{
    sHellStmt *e;
    char *test[] = {
        "call   \"c\"  , 3  ",
        "call\"c\"  , 3  ",
        "call\"c\"    ",
        "a=   \"c\"    ",
        "a   =   \"c\"    ",
        "a   =\"c\"    ",
        "callA   \"c b\"  , 3  ;callB\"c\"  , 3  ;  a=   \"c\"    ",
        "a   =0x8    ",
        "a   = 0x10    ",
        "a=0xA;    b   = 0x10;; ; ;",
        "a=0;; ;",
        "a= \"   2\";; ;",
        "cA ;; cB ;",
    };
    int i;

    for (i=0;i<sizeof(test)/sizeof(*test);++i) {
        printf("===============\n");
        printf("stmt: %s\n", test[i]);
        e = sHellParse(test[i]);
        sHellEvaluate(e);
    }

    return 0;
}
