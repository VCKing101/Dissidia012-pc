#include "menu/auto_region3.h"

/* ScreenC4560: restarts the list with ids 0x1A, 0x19 (and 0x18, 0x17 when func_08867894 is 0). */
void func_08ABC75C(ScreenC4560 *self)
{
    int busy;

    self->work->field_B08 = 0;
    func_08ABC828(self, 0x1A);
    func_08ABC828(self, 0x19);
    busy = func_08867894();
    if (busy == 0) {
        func_08ABC828(self, 0x18);
        func_08ABC828(self, 0x17);
    }
    func_08ABC868(self);
}
