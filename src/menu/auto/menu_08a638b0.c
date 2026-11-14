#include "menu/auto_region2.h"

extern unsigned char D_08A67E38[];

/* Page base constructor. */
Page *func_08A638B0(Page *self)
{
    self->vtable = D_08A67E38;
    return self;
}
