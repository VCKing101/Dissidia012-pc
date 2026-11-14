#include "main/auto_region4.h"

/* Obj8FBBE0: clears flag 2 and plays the closing scripts. */
void func_088FC220(Obj8FBBE0 *self)
{
    self->work->flags = self->work->flags & 0xfffffffd;
    func_088D3EA0(self->work->layout, self->work->handles, D_089CCE9C);
    func_088D3C24(self->work->layout, self->work->handles, D_089CCE38, 0x120);
    func_088FBC08(self);
    func_088FBF8C(self);
}
