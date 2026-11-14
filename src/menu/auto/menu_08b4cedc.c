#include "menu/auto_region4.h"

extern unsigned char D_08B4EA68[];

/* Part4CEDC constructor. */
Part4CEDC *func_08B4CEDC(Part4CEDC *self)
{
    func_08931D68(self);
    self->vtable = D_08B4EA68;
    func_08B4CF18(self);
    return self;
}
