#include "menu/auto_region1.h"

/* Menu root: clears flag 4; state 3 once func_088F7B84(field_6964) reports
 * so. */
void func_08A1DD08(MenuRoot *self)
{
    int done;

    self->work->flags = self->work->flags & ~4;
    done = func_088F7B84(self->work->field_6964);
    if (done != 0) {
        self->work->state = 3;
    }
}
