#include "battle/auto_region5.h"

/* Part47170: in phase 1, runs func_088D3F00 over the handles. */
void func_08B47214(Part47170 *self)
{
    if (self->phase == 1) {
        func_088D3F00(0, self->handles, D_08B47328);
    }
}
