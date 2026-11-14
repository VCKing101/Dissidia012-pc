#include "menu/auto_region2.h"

/* Screen6D710: sends event 9 to its machine. */
void func_08A6D100(Screen6D710 *self)
{
    func_08A79EF0(self->field_1B0, self, 9, 0, 0);
}
