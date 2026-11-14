#include "menu/auto_region2.h"

/* Panel58420: plays motion 5 on handle 2, hides handle 3. */
void func_08A57EB8(Panel58420 *self)
{
    func_088D9F5C(self->handles[2], 5, 1);
    func_088D9CFC(self->handles[3]);
}
