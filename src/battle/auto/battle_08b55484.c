#include "battle/auto_region5.h"

/* Frees field_0C, field_10 and field_18 back to the heap and forgets the
 * heap. */
void func_08B55484(Part55484 *self)
{
    func_0885C538(self->heap, self->field_0C, 0);
    self->field_0C = 0;
    func_0885C538(self->heap, self->field_10, 0);
    self->field_10 = 0;
    func_0885C538(self->heap, self->field_18, 0);
    self->field_18 = 0;
    self->heap = 0;
}
