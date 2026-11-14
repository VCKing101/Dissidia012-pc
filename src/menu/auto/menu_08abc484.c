#include "menu/auto_region3.h"

/* ScreenC4560: restarts the list with ids 0x12..0xD, 8 and 7. */
void func_08ABC484(ScreenC4560 *self)
{
    self->work->field_B08 = 0;
    func_08ABC828(self, 0x12);
    func_08ABC828(self, 0x11);
    func_08ABC828(self, 0x10);
    func_08ABC828(self, 0xF);
    func_08ABC828(self, 0xE);
    func_08ABC828(self, 0xD);
    func_08ABC828(self, 8);
    func_08ABC828(self, 7);
    func_08ABC868(self);
}
