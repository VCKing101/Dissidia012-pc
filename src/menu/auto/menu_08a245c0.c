#include "menu/auto_region1.h"

/* Screen28560: sets up cursor field_2044 over field_25A8 entries and moves
 * it to the last one. */
void func_08A245C0(Screen28560 *self)
{
    unsigned int *flags;
    unsigned int count;

    func_088D46E4(&self->work->field_2044, 0, 0, self->work->field_25A8 - 1, 0, 8);
    flags = &self->work->field_2044.flags;
    *flags = *flags | 4;
    count = self->work->field_25A8;
    if (count != 0) {
        func_088D4928(&self->work->field_2044, count - 1);
    }
}
