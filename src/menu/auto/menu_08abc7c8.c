#include "menu/auto_region3.h"

/* ScreenC4560: restarts the list with id 0x1B (and 0x18, 0x17 when func_08867894 is 0). */
void func_08ABC7C8(ScreenC4560 *self)
{
    int busy;

    self->work->field_B08 = 0;
    func_08ABC828(self, 0x1B);
    busy = func_08867894();
    if (busy == 0) {
        func_08ABC828(self, 0x18);
        func_08ABC828(self, 0x17);
    }
    func_08ABC868(self);
}
