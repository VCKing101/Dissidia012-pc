#include "menu/auto_region1.h"

/* Menu root: makes a text from its messages, all arguments passed on. */
void func_08A1D460(MenuRoot *self, int kind, int arg1, int arg2, int arg3, int arg4)
{
    func_0887B3EC(self->work->messages, kind, arg1, arg2, arg3, arg4);
}
