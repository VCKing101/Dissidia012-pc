#include "menu/auto_region4.h"

extern unsigned char D_08B4EA98[];

/* Part4D4DC constructor. */
Part4D4DC *func_08B4D4DC(Part4D4DC *self)
{
    func_08931D68(self);
    self->vtable = D_08B4EA98;
    func_08B4D518(self);
    return self;
}
