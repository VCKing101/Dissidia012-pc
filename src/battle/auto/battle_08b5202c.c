#include "battle/auto_region5.h"

/* Part5202C constructor: points it at its fixed tables. */
Part5202C *func_08B5202C(Part5202C *self)
{
    self->field_00 = 1;
    self->field_08 = D_08B52900;
    func_08B51F38(&self->field_0C, D_08B529A0, 0x10);
    func_08B51F38(&self->field_14, D_08B52AE0, 0xE);
    func_08B51F38(&self->field_1C, D_08B52BF8, 0xB);
    return self;
}
