#include "menu/auto_region2.h"

/* Screen4C0FC: in mode 1, stops D_089D7260 and frees work field_04. */
void func_08A49E10(Screen4C0FC *self)
{
    Work4C0FC *work;

    if (self->work->field_08 == 1) {
        func_088AF044(D_089D7260);
        func_088AEF98(D_089D7260);
        work = self->work;
        if (work->field_04 != 0) {
            func_0885C538(self->heap, work->field_04, 0);
            self->work->field_04 = 0;
            work = self->work;
        }
        work->field_08 = 0;
    }
}
