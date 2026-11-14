#include "menu/auto_region1.h"

/* Screen38EA8: sets up cursor field_C4 over func_08A37024() entries. */
void func_08A36CC4(Screen38EA8 *self)
{
    int count;
    unsigned int *flags;

    count = func_08A37024(self);
    func_088D46E4(&self->work->field_C4, 0, 0, count - 1, 0, count);
    flags = &self->work->field_C4.flags;
    *flags = *flags | 1;
}
