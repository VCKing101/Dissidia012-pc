#include "battle/auto_region5.h"

/* Frees field_20 and field_24 back to the heap and forgets the heap. */
void func_08B4F6EC(Part4F6EC *self)
{
    func_0885C538(self->heap, self->field_20, 0);
    self->field_20 = 0;
    func_0885C538(self->heap, self->field_24, 0);
    self->field_24 = 0;
    self->heap = 0;
}
