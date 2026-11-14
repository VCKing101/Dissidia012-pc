#include "battle/auto_region2.h"

/* Unit036D8 reset. */
void func_08A02258(Unit036D8 *self)
{
    self->field_04 = 0;
    {
        int i;
        for (i = 0; i < 2; i++) {
            self->field_08[i] = 0;
        }
    }
    self->field_10 = -1;
    self->field_14 = 0;
}
