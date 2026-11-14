#include "menu/auto_region2.h"

/* Screen4FFF4: opens Panel61C40 on the data block +0x6FFC. */
void func_08A4D79C(Screen4FFF4 *self, unsigned char flag)
{
    unsigned int value;
    Work4FFF4 *work;

    value = func_08A60B30(self->work->field_6FD4);
    func_08A4ED34(self, 0x19, value);
    func_08A697B0(self->work->field_6FF8, 1, -1);
    work = self->work;
    func_08A6082C(work->field_6FD4, &work->field_6FFC, work->messages, work->field_6B31);
    if (flag != 0) {
        func_08A60A9C(self->work->field_6FD4);
    }
}
