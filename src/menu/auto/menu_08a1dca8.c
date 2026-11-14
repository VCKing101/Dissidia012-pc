#include "menu/auto_region1.h"

/* Menu root: clears flag 4 and sets 0x2000 when func_088F7B68(field_6964)
 * reports so. */
void func_08A1DCA8(MenuRoot *self)
{
    int result;

    result = func_088F7B68(self->work->field_6964);
    self->work->flags = self->work->flags & ~4;
    if (result != 0) {
        self->work->flags = self->work->flags | 0x2000;
    }
}
