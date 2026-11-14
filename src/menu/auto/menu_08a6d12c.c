#include "menu/auto_region2.h"

/* Screen6D710: sends event 10 with value (4 bytes) to its machine. */
void func_08A6D12C(Screen6D710 *self, int value)
{
    func_08A79EF0(self->field_1B0, self, 10, value, 4);
}
