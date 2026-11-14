#include "battle/auto_region5.h"

/* Part212C8 constructor. */
Part212C8 *func_08B20A88(Part212C8 *self)
{
    self->vtable = D_08B212C8;
    func_08993560(self->field_0C, 2, sizeof(Part20A68), func_08B1FE68);
    func_08B20B50(self);
    return self;
}
