#include "battle/auto_region4.h"

/* PartB71A0 constructor: builds the base (kind 6), then resets. */
PartB71A0 *func_08AB67DC(PartB71A0 *self, int arg)
{
    func_0883975C(self, 6, arg);
    self->vtable = D_08AB71A0;
    func_08AB6884(self);
    return self;
}
