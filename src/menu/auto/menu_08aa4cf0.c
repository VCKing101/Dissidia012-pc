#include "menu/auto_region3.h"

/* ScreenA7ED0: with flag 1, plays motion 1 (arg) or 2 (no arg) on handle 6. */
void func_08AA4CF0(ScreenA7ED0 *self, int arg)
{
    MenuModel *model;

    if ((self->work->flags & 1) != 0) {
        model = self->work->handles[6];
        if (arg == 0) {
            func_088D9F5C(model, 2, 1);
        }
        else {
            func_088D9F5C(model, 1, 1);
        }
    }
}
