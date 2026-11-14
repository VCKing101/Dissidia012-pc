#include "menu/auto_region2.h"

/* Panel6B170: func_08A6A0F4, stops its layout, then the panel. */
void func_08A69DF4(Panel6B170 *self)
{
    func_08A6A0F4(self);
    func_088D3A04(&self->layout);
    func_08A51EB8(&self->base);
}
