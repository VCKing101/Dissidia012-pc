#include "menu/auto_region1.h"

/* Menu root: opens its prompt with value. */
void func_08A1E144(MenuRoot *self, int value)
{
    func_088D4E00(value, &self->work->prompt);
}
