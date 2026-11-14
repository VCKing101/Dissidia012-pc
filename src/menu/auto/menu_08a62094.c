#include "menu/auto_region2.h"

/* Panel63680: opens the panel base, then func_08A6282C. */
void func_08A62094(Panel63680 *self, int arg1, int arg2, int arg3)
{
    func_08A51E64(&self->base, arg1, arg2, arg3);
    func_08A6282C(self);
}
