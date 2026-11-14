#include "menu/auto_region2.h"

/* Screen4C0FC: in mode 2, stops D_089D7260. */
void func_08A49F7C(Screen4C0FC *self)
{
    if (self->work->field_08 == 2) {
        func_088AF044(D_089D7260);
        func_088AEF98(D_089D7260);
        self->work->field_08 = 0;
    }
}
