#include "battle/auto_region2.h"

/* Part01DA8 constructor. */
void *func_08A01788(void *object)
{
    Part01DA8 *self = object;

    self->vtable = D_08A01DA8;
    func_08A01818(self);
    return self;
}
