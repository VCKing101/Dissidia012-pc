#include "menu/auto_region3.h"

/* ScreenDB30: drops the pick (field_DB10), hides handle 0x22, cursor over field_100C. */
void func_08AD0BA4(ScreenDB30 *self)
{
    WorkDB30 *work;

    self->work->field_DB10 = -1;
    func_088D9CFC(self->work->handles[0x22]);
    work = self->work;
    func_088D4788(&work->cursor, work->field_100C, work->field_10F0);
    func_08ACB680(self);
}
