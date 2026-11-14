#include "menu/auto_region3.h"

extern unsigned char D_08AD8E50[];

/* ScreenDB30 deleting destructor. */
void func_08AD0ED0(ScreenDB30 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08AD8E50;
        func_08ACF434(self);
        func_08AF1244(self, 0);
        if ((flags & 1) != 0) {
            func_08AF12B0(self);
        }
    }
}
