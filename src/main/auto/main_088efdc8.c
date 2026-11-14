#include "main/auto_region4.h"

/* Obj8EFDC8: clears it, then stores the three shorts. */
void func_088EFDC8(Obj8EFDC8 *self, unsigned short arg1, unsigned short arg2, unsigned short arg3)
{
    func_08935828(self, 0, 0x10);
    self->field_00 = arg1;
    self->field_02 = arg2;
    self->field_04 = arg3;
}
