#include "battle/auto_region1.h"

/* UnitF1B20 constructor. */
UnitF1B20 *func_089F1488(UnitF1B20 *self)
{
    func_089F12F0(&self->base);
    self->base.vtable = D_089F1B20;
    func_089F1BD8(&self->field_14);
    func_089F153C(self);
    return self;
}
