#include "menu/auto_region2.h"

extern unsigned char D_08A52318[];

/* Widget base constructor. */
Widget *func_08A52300(Widget *self)
{
    self->vtable = D_08A52318;
    return self;
}
