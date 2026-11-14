#include "menu/auto_region3.h"

/* Step7ADD8: on input bit 2, plays the leave motions; state 3. */
void func_08A7B134(Step7ADD8 *self, unsigned int flags)
{
    if ((flags & 2) != 0) {
        func_08A771B8(&self->field_04);
        self->state = 3;
    }
}
