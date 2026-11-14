#include "battle/auto_region3.h"

/* Part622FC reset. */
void func_08A622FC(Part622FC *self)
{
    self->active = 0;
    func_08935828(self->field_10, 0, 0x120);
}
