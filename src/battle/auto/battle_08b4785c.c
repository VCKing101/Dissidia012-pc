#include "battle/auto_region5.h"

/* Part4785C constructor. */
Part4785C *func_08B4785C(Part4785C *self)
{
    func_0892ABC0(self);
    self->vtable = D_08B48448;
    func_08B49220(&self->field_58);
    func_08B47170(&self->field_B0);
    func_08993560(self->entries, 0xE, sizeof(Part47358), func_08B47358);
    func_08B478C0(self);
    return self;
}
