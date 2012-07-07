#include <stdio.h>
#include <assert.h>

#include "sHell.h"

static
void
sHellTestAssign(const char *name, const sHellStmt *arg)
{
    printf("%s = ", name);
    switch (arg->type) {
        case sHellNum:
            printf("%u", arg->num);
            break;
        case sHellStr:
            printf("'%s'", arg->str);
            break;
        default:
            printf("(undef)");
            break;
    }
    printf("\n");
}

static
void
sHellTestCall(const char *name, const sHellStmt *arg)
{
    printf("%s(", name);
    while (arg) {
        switch (arg->type) {
            case sHellNum:
                printf("%u", arg->num);
                break;
            case sHellStr:
                printf("'%s'", arg->str);
                break;
            default:
                printf("(undef)");
                break;
        }
        arg = arg->next;

        if (arg)
            printf(", ");
    }
    printf(")\n");
}


int main(void)
{
    sHellStmt *e;
    char *test[] = {
        "call   \"c\"  , 3  ",
        "call   \"c\"  , 3, 4,5 ,0  ",
        "call\"c\"  , 3  ",
        "call\"c\"    ",
        "a=   \"c\"    ",
        "a   =   \"c\"    ",
        "a   =\"c\"    ",
        "callA   \"c b\"  , 3  ;callB\"c\"  , 3  ;  a=   \"c\"    ",
        "a   =0x8    ",
        "a   = 0x10    ",
        "a=0xA; ; ;  b   = 0x10;; ; ;",
        "a=0;; ;",
        "a=0,1",            /* should be syntax error */
        "a= \"   2\";; ;",
        "cA ;; cB ;",
        "#cA ;; cB ;",
        "cA ;#; a=4 ;",
        "cA ;a=3; b=2#4 ;",
    };
    int i;

    sHellSetAssignCbk(sHellTestAssign);
    sHellSetCallCbk(sHellTestCall);

    for (i=0;i<sizeof(test)/sizeof(*test);++i) {
        printf("===============\n");
        printf("stmt: `%s`\n", test[i]);
        sHellParse(test[i]);
    }

    return 0;
}
