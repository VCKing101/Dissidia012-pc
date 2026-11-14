#include "menu/auto_region2.h"

/* Panel58420: plays motion 2 on handle 4, state 4. */
void func_08A5776C(Panel58420 *self)
{
    func_088D9F5C(self->handles[4], 2, 1);
    self->state = 4;
}
