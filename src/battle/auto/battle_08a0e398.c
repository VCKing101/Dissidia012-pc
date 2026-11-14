#include "battle/auto_region2.h"

/* Part0EB78 constructor. */
void *func_08A0E398(void *object)
{
    Part0EB78 *self = object;

    self->vtable = D_08A0EB78;
    func_089F7C48(&self->field_08);
    func_08A0E43C(self);
    return self;
}
