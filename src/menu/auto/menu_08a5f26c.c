#include "menu/auto_region2.h"

/* Panel5FA00: plays motion 3 on handle 0, state 4. */
void func_08A5F26C(Panel5FA00 *self)
{
    func_088D9F5C(self->handles[0], 3, 1);
    self->state = 4;
}
