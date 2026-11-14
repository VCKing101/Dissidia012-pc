#include "menu/auto_region4.h"

extern unsigned char D_08B4EAC8[];

/* Widget4DF58 constructor: builds its four parts. */
Widget4DF58 *func_08B4DF58(Widget4DF58 *self)
{
    func_0892ABC0(self);
    self->vtable = D_08B4EAC8;
    func_08931E7C(self->field_40);
    func_08B4CA98(&self->field_64);
    func_08B4CEDC(&self->field_84);
    func_08B4D4DC(&self->field_D8);
    func_08B4E054(self);
    return self;
}
