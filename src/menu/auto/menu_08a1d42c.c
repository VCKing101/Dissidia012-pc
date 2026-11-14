#include "menu/auto_region1.h"

/* Menu root: makes a text from its messages (kind 1). */
void func_08A1D42C(MenuRoot *self, int arg1, int arg2, int arg3, int arg4)
{
    func_0887B3EC(self->work->messages, 1, arg1, arg2, arg3, arg4);
}
