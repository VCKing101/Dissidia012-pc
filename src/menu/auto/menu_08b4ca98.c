#include "menu/auto_region4.h"

extern unsigned char D_08B4EA38[];

/* Part4CA98 constructor. */
Part4CA98 *func_08B4CA98(Part4CA98 *self)
{
    func_08931D68(self);
    self->vtable = D_08B4EA38;
    func_08B4CAD4(self);
    return self;
}
