#include "main/agent26_types.h"

extern "C" {
void func_08863C2C(int result, A26Device *dev)
{
    A26Request *r = &dev->request;
    r->state = 0;
    dev->finish(r->value);
    if (r->done != 0) {
        r->done(result, r->doneArg);
    }
}
}
