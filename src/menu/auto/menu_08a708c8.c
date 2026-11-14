#include "menu/auto_region2.h"

/* Obj708C8: func_08A702D0, then resets its three members. */
void func_08A708C8(Obj708C8 *self)
{
    func_08A702D0(self);
    func_08A7059C(self->field_148);
    func_08A7084C(&self->field_FE8);
    func_08A7085C(self->field_FF0);
}
