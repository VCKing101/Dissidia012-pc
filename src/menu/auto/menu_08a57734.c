#include "menu/auto_region2.h"

/* Panel58420: plays motion 4 on handle 2, state 3. */
void func_08A57734(Panel58420 *self)
{
    func_088D9F5C(self->handles[2], 4, 1);
    self->state = 3;
}
