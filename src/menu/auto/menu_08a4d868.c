#include "menu/auto_region2.h"

/* Screen4FFF4: opens Panel63680 on block +0x7020, state 7 (6 with flag). */
void func_08A4D868(Screen4FFF4 *self, unsigned char flag)
{
    unsigned int value;
    Work4FFF4 *work;

    value = func_08A6233C(self->work->field_6FD8);
    func_08A4ED34(self, 0x19, value);
    func_08A697B0(self->work->field_6FF8, 2, -1);
    work = self->work;
    func_08A62128(work->field_6FD8, &work->field_7020, work->messages);
    self->work->state = 7;
    if (flag != 0) {
        self->work->field_6FD8->field_E45 = 1;
        func_08A4CFD4(self, 0);
        self->work->state = 6;
    }
}
