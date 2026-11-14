#include "menu/auto_region3.h"

/* Page76574 constructor. */
Page76574 *func_08A76574(Page76574 *self)
{
    func_088D2E70(&self->field_00);
    func_088D31C4(&self->field_04);
    func_08993560(self->rows, 2, sizeof(Row75D14), func_08A75D14);
    func_08A76158(&self->list);
    func_08A76344(&self->field_1CC);
    func_08A73C38(&self->field_1D8);
    self->field_1DC = 0;
    return self;
}
