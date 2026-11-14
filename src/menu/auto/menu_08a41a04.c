#include "menu/auto_region2.h"

/* MenuRoot2: sets up and starts the object at +0x13C. */
void func_08A41A04(MenuRoot2 *self)
{
    func_088D5318(self->field_13C, 0x39, 0xE1000);
    func_088D53BC(self->field_13C);
}
