#include "menu/auto_region3.h"

/* PanelC18 constructor: points work at its own data and clears it. */
PanelC18 *func_08AC87F4(PanelC18 *self)
{
    self->work = &self->data;
    func_08935828(self, 0, 0xC18);
    return self;
}
