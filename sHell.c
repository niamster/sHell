#include <stdlib.h>

#include "sHell.h"

void *sHellDfltAlloc(size_t sz);
void sHellDfltFree(void *p);

static sHellAllocCbk    _sHellAlloc   = sHellDfltAlloc;
static sHellFreeCbk     _sHellFree    = sHellDfltFree;

sHellAllocCbk
sHellSetAllocCbk(sHellAllocCbk cbk)
{
    sHellAllocCbk old = _sHellAlloc;

    _sHellAlloc = cbk;

    return old;
}

sHellFreeCbk
sHellSetFreeCbk(sHellFreeCbk cbk)
{
    sHellFreeCbk old = _sHellFree;

    _sHellFree = cbk;

    return old;
}

void *sHellAlloc(size_t sz)
{
    return _sHellAlloc(sz);
}

void sHellFree(void *p)
{
    _sHellFree(p);
}

void *sHellDfltAlloc(size_t sz)
{
    return malloc(sz);
}

void sHellDfltFree(void *p)
{
    free(p);
}
