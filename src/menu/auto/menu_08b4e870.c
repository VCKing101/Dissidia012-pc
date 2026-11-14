#include "menu/auto_region4.h"

extern const short D_08B4EC58[];

void func_08B4E870(Widget4DF58 *self)
{
    if (self->state == 1) {
        func_088D3F00(0, self->handles, D_08B4EC58);
        func_08931E18(self->field_40);
        func_08931E18(&self->field_64);
        func_08931E18(&self->field_84);
        func_08B4D728(&self->field_D8);
    }
}
