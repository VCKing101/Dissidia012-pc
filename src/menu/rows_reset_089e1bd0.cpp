#include "menu/agent29_types.h"

/* ScreenE75C4: hides the icons of all seven rows, then plays motion 0 on
   the icon of the current row and marks it 1. */
extern "C" void func_089E1BD0(ScreenE75C4 *self)
{
    int one = 1;

    for (int i = 0; i < 7; i++) {
        func_088D9CFC(self->work->handles[D_089E7518[i].icon]);
    }
    func_088D9F5C(self->work->handles[D_089E7518[WORK_INT(self->work, 0xA2C)].icon], 0, one);
    MENU_MODEL_STATE(self->work->handles[D_089E7518[WORK_INT(self->work, 0xA2C)].icon]) = 1;
}
