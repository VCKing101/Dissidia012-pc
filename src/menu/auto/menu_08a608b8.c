#include "menu/auto_region2.h"

/* Panel61C40: plays motion 2 on handle 0, state 4. */
void func_08A608B8(Panel61C40 *self)
{
    func_088D9F5C(self->handles[0], 2, 1);
    self->state = 4;
}
