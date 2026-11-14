#include "battle/auto_region2.h"

/* Part02198 constructor. */
void *func_08A01DD0(void *object)
{
    Part02198 *self = object;

    self->vtable = D_08A02198;
    func_08A01E60(self);
    return self;
}
