#include "menu/auto_region2.h"

extern unsigned char D_08A522A0[];

/* Panel base constructor. */
Panel *func_08A51DD0(Panel *self)
{
    self->vtable = D_08A522A0;
    func_08A51E3C(self);
    return self;
}
