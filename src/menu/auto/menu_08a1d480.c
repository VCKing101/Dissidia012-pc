#include "menu/auto_region1.h"

/* Menu root: makes a text from its messages (kind 5) for value. */
void func_08A1D480(MenuRoot *self, int value)
{
    func_0887B3EC(self->work->messages, 5, 0, value, 0, 0);
}
