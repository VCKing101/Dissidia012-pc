#include "menu/auto_region3.h"

/* ScreenDB30: places handle 0x23 at (0x9A, field_FAC - field_FB8). */
void func_08ACC020(ScreenDB30 *self)
{
    WorkDB30 *work;

    work = self->work;
    func_088D3AE8(work->handles, 0x9A, work->field_FAC - work->field_FB8, 0x23);
}
