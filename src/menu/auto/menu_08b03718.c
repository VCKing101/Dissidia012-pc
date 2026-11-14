#include "menu/auto_region4.h"

extern unsigned char D_08B044F8[];

/* Deleting destructor of the panel with the vtable D_08B044F8. */
void func_08B03718(PanelBase *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08B044F8;
        func_08AF1244(self, 0);
        if ((flags & 1) != 0) {
            func_08AF12B0(self);
        }
    }
}
