#include "menu/auto_region3.h"

/* ScreenDB30: picks the cursor row (field_DB10), plays motion 0x1E on handle 0x22, cursor over field_10F8. */
void func_08AD0B2C(ScreenDB30 *self)
{
    WorkDB30 *work;

    func_08AD0A58(self);
    self->work->field_DB10 = self->work->cursor.index;
    func_088D9F5C(self->work->handles[0x22], 0x1E, 1);
    work = self->work;
    func_088D4788(&work->cursor, work->field_10F8, work->field_11DC);
    func_08ACB680(self);
}
