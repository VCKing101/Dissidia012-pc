#include "battle/auto_region5.h"

/* PartF6008: runs func_0887B944 on model 3, if any. */
void func_08AF615C(PartF6008 *self)
{
    if (self->models[3] != 0) {
        func_0887B944(self->models[3]);
    }
}
