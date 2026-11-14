#include "battle/auto_region3.h"

/* Part25AD8 constructor. */
Part25AD8 *func_08A255B0(Part25AD8 *self)
{
    self->vtable = D_08A25AD8;
    func_089F8208(&self->field_14);
    func_08A25AF0(&self->field_4C);
    func_08A25668(self);
    return self;
}
