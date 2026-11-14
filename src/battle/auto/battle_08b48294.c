#include "battle/auto_region5.h"

/* Part4785C: reloads the four handles from the first resource. */
void func_08B48294(Part4785C *self)
{
    int i;

    func_08B48300(self);
    for (i = 0; i < 4; i++) {
        self->handles[i] = func_088DAA68(self->resources[0]);
    }
    func_088D3CF0(0, self->handles, D_08B484C0);
}
