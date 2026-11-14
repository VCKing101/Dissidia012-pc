#include "battle/auto_region5.h"

/* Frees field_1C and field_20 back to the heap and forgets the heap. */
void func_08B4DEA4(Part4DEA4 *self)
{
    func_0885C538(self->heap, self->field_1C, 0);
    self->field_1C = 0;
    func_0885C538(self->heap, self->field_20, 0);
    self->field_20 = 0;
    self->heap = 0;
}
