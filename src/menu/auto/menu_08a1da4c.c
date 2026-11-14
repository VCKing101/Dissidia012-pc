#include "menu/auto_region1.h"

/* Menu root: when func_08A1D8E4 reports so, func_088F7988 on field_6964 and
 * func_08900BE4 on D_089D8140. */
void func_08A1DA4C(MenuRoot *self)
{
    int ready;

    ready = func_08A1D8E4(self);
    if (ready != 0) {
        func_088F7988(self->work->field_6964);
        func_08900BE4(D_089D8140);
    }
}
