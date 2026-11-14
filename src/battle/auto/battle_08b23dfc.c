#include "battle/auto_region5.h"

/* Part2368C: sets field_19F; when it is 1, also loads field_1A4 from
 * field_04. */
unsigned int func_08B23DFC(Part2368C *self, unsigned char enable)
{
    self->field_19F = enable;
    if (enable == 1) {
        self->field_1A4 = func_088CA42C(self->field_04);
    }
    return 0;
}
