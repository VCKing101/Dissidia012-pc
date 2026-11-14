#include "menu/auto_region3.h"

/* ScreenA690: plays motion 2 on handle 0x80 if it shows something. */
void func_08AE638C(ScreenA690 *self)
{
    MenuModel *model;

    model = self->work->handles[0x80];
    if (model->field_0C != -1) {
        func_088D9F5C(model, 2, 1);
    }
}
