#include "menu/auto_region2.h"

/* Panel61C40: closes (func_08A61468 first with flag), state 2. */
void func_08A60950(Panel61C40 *self, unsigned char flag)
{
    if (flag != 0) {
        func_08A61468(self);
    }
    self->state = 2;
}
