#include "main/agent26_types.h"

extern "C" {
void func_08863C2C(int result, A26Device *dev);

void func_08863BAC(int result, void *context)
{
    A26Device *dev = (A26Device *)context;
    A26Request *r = &dev->request;
    r->state = 0;
    if (result >= 0) {
        r->value = result;
        dev->open(result, func_08863C2C, (A26Device *)context);
    } else {
        if (r->done != 0) {
            r->done(0, r->doneArg);
        }
    }
}
}
