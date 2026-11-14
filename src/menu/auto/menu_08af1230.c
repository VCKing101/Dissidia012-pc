#include "menu/auto_region3.h"

extern unsigned char D_08AF12E0[];

/* PanelBase constructor. */
PanelBase *func_08AF1230(PanelBase *self)
{
    self->vtable = D_08AF12E0;
    return self;
}
