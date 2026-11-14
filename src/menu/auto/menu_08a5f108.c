#include "menu/auto_region2.h"

/* Panel5FA00: opens the panel base, sets up its cursors and stamps the time. */
void func_08A5F108(Panel5FA00 *self, int arg1, int arg2, int arg3)
{
    func_08A51E64(&self->base, arg1, arg2, arg3);
    func_08A5F7CC(self);
    self->field_320 = func_088729A0(0);
}
