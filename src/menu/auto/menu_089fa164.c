#include "menu/auto_region1.h"

/* Screen0B6A0: func_089FB4A4 on its layout, then state 0x11. */
void func_089FA164(Screen0B6A0 *self)
{
    Work0B6A0 *work;

    work = self->work;
    func_089FB4A4(self, &work->layout, work->handles, work->texts);
    self->work->state = 0x11;
}
