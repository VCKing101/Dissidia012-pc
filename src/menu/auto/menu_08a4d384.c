#include "menu/auto_region2.h"

/* Screen4FFF4: hands the edited name on by work field_6D38 (2, 3 or 4). */
void func_08A4D384(Screen4FFF4 *self)
{
    Work4FFF4 *work;
    int kind;

    work = self->work;
    kind = work->field_6D38;
    if (kind == 2) {
        func_089298F8(work->field_10, work->field_6B32);
    } else if (kind == 3) {
        func_08929928(work->field_10, work->field_6B32);
    } else if (kind == 4) {
        func_08935524(work->field_6E8C, work->field_6B32, 0x66);
        self->work->field_6E8C->field_64 = 0;
        self->work->field_6E8C = 0;
    }
}
