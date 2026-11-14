#include "menu/auto_region2.h"

extern unsigned char D_08A51D98[];

/* Action constructor. */
Action *func_08A51D38(Action *self)
{
    self->vtable = D_08A51D98;
    func_08A51D70(self);
    return self;
}
