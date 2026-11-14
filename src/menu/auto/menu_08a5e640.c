#include "menu/auto_region2.h"

/* Panel5EED8: plays motion 3 on handle 6, state 3. */
void func_08A5E640(Panel5EED8 *self)
{
    func_088D9F5C(self->handles[6], 3, 1);
    self->state = 3;
}
