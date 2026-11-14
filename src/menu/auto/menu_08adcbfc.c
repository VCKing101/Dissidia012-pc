#include "menu/auto_region3.h"

extern unsigned char D_08ADF498[];

/* Screen46AC deleting destructor. */
void func_08ADCBFC(Screen46AC *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08ADF498;
        func_08ADC9D4(self);
        func_08AF1244(self, 0);
        if ((flags & 1) != 0) {
            func_08AF12B0(self);
        }
    }
}
