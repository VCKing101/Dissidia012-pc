#include "battle/auto_region5.h"

/* Frees field_44 back to the heap, frees the three objects at field_28 and
 * forgets the heap. */
void func_08B5C92C(Part5C92C *self)
{
    int i;

    func_0885C538(self->heap, self->field_44, 0);
    self->field_44 = 0;
    for (i = 0; i < 3; i++) {
        func_08842138(self->field_28[i].object);
        self->field_28[i].object = 0;
    }
    self->heap = 0;
}
