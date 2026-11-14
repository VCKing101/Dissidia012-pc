#include "menu/auto_region1.h"

/* Screen35A00: frees the task's block and forgets its heap. */
void func_08A310A8(Screen35A00 *self)
{
    func_0885C538(self->heap, self->field_00, 0);
    self->field_00 = 0;
    self->heap = 0;
}
