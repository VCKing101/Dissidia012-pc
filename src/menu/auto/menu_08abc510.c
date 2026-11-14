#include "menu/auto_region3.h"

/* ScreenC4560: restarts the list with ids 0x1A, 0x19, 0x14, 0x13, 2 and 1. */
void func_08ABC510(ScreenC4560 *self)
{
    self->work->field_B08 = 0;
    func_08ABC828(self, 0x1A);
    func_08ABC828(self, 0x19);
    func_08ABC828(self, 0x14);
    func_08ABC828(self, 0x13);
    func_08ABC828(self, 2);
    func_08ABC828(self, 1);
    func_08ABC868(self);
    func_08ABC868(self);
    func_08ABC868(self);
}
