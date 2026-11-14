#include "menu/auto_region1.h"

/* Menu root: looks up message id (kind 1). */
void func_08A1D4B4(MenuRoot *self, int id)
{
    func_0887B500(self->work->messages, 1, id);
}
