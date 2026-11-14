#include "menu/auto_region3.h"

/* Obj7525C: binds its three nodes to a layout and keeps a value. */
void func_08A7525C(Obj7525C *self, void *source, int value)
{
    func_088D2EC8(&self->field_00, source, 0x13);
    func_088D2EC8(&self->field_04, source, 0x14);
    func_088D2EC8(&self->field_08, source, 0x12);
    self->field_10 = value;
}
