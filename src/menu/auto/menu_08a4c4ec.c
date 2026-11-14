#include "menu/auto_region2.h"

/* Screen4FFF4: shuts down the Obj3A864 in work field_7184 and frees it. */
void func_08A4C4EC(Screen4FFF4 *self)
{
    func_08A3AA54(self->work->field_7184);
    func_0885C538(self->heap, self->work->field_7184, 0);
}
