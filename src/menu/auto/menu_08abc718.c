#include "menu/auto_region3.h"

/* ScreenC4560: restarts the list with ids 10 and 9. */
void func_08ABC718(ScreenC4560 *self)
{
    self->work->field_B08 = 0;
    func_08ABC828(self, 10);
    func_08ABC828(self, 9);
    func_08ABC868(self);
}
