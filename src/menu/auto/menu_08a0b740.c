#include "menu/auto_region1.h"

/* Delegate0B920 constructor. */
Delegate0B920 *func_08A0B740(Delegate0B920 *self, Screen0B6A0 *owner, int value)
{
    self->vtable = D_08A0B920;
    self->owner = owner;
    self->field_08 = value;
    return self;
}
