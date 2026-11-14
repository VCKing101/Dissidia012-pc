#include "main/auto_region4.h"

/* Obj8FBBE0: sets flag 2 and plays the opening scripts. */
void func_088FC194(Obj8FBBE0 *self)
{
    self->work->flags = self->work->flags | 2;
    func_088D3B88(self->work->layout, self->work->handles, D_089CCE38, 0, 0x120);
    func_088D3CF0(self->work->layout, self->work->handles, D_089CCE9C);
    if (self->work->field_12 == 1) {
        func_088D9F5C(self->work->handles[2], 0, 1);
    }
}
