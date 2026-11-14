#include "menu/auto_region1.h"

/* Screen28560: hides handles 0x44 and 0x45. */
void func_08A246BC(Screen28560 *self)
{
    func_088D9CFC(self->work->handles[0x44]);
    func_088D9CFC(self->work->handles[0x45]);
}
