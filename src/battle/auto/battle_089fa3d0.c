#include "battle/auto_region1.h"

/* UnitFAAC0 constructor. */
UnitFAAC0 *func_089FA3D0(UnitFAAC0 *self)
{
    func_089F12F0(&self->base);
    self->base.vtable = D_089FAAC0;
    func_08993560(self->field_10, 2, 0x118, func_08A174F0);
    func_089FA4A8(self);
    return self;
}
