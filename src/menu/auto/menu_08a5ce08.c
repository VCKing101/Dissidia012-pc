#include "menu/auto_region2.h"

/* Panel5DB80: plays motion 3 on handle 1, state 3. */
void func_08A5CE08(Panel5DB80 *self)
{
    func_088D9F5C(self->handles[1], 3, 1);
    self->state = 3;
}
