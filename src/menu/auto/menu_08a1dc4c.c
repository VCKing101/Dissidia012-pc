#include "menu/auto_region1.h"

/* Menu root: once func_088F7B4C(field_6964) reports so, clears flag 2 and
 * goes to state 3. */
void func_08A1DC4C(MenuRoot *self)
{
    int done;

    done = func_088F7B4C(self->work->field_6964);
    if (done != 0) {
        self->work->flags = self->work->flags & ~2;
        self->work->state = 3;
    }
}
