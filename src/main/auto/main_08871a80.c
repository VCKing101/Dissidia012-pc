#include "main/auto_region2.h"

/* Calls the callback with arg and its data. */
void func_08871A80(Obj871A04 *self, int arg)
{
    if (self->callback != 0) {
        self->callback(arg, self->data);
    }
}
