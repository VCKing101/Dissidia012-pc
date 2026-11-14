#include "battle/auto_region5.h"

/* Frees field_0C back to the heap and forgets the heap. */
void func_08B55BD4(Part55484 *self)
{
    func_0885C538(self->heap, self->field_0C, 0);
    self->field_0C = 0;
    self->heap = 0;
}
