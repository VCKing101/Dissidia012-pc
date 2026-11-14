#include "battle/auto_region4.h"

/* PartC6098: frees the three handles. */
void func_08AC6218(PartC6098 *self)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (self->handles[i] != 0) {
            func_0899BB1C(self->handles[i]);
        }
        self->handles[i] = 0;
    }
}
