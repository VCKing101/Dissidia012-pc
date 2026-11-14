#include "menu/auto_region2.h"

/* Screen4FFF4: forwarder to Panel5FA00 (func_08A5F140). */
void func_08A4E2C4(Screen4FFF4 *self, int value)
{
    func_08A5F140(self->work->field_6FDC, value);
}
