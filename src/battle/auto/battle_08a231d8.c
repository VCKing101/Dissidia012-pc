#include "battle/auto_region3.h"

/* Part24BA0 constructor. */
Part24BA0 *func_08A231D8(Part24BA0 *self)
{
    self->vtable = D_08A24BA0;
    func_089EEA00(&self->field_10);
    func_089EEA00(&self->field_80);
    func_08A23298(self);
    return self;
}
