#include "battle/auto_region4.h"

/* Part88B28 deleting destructor: destroys the members in reverse order. */
void func_08A871B8(void *object, int flags)
{
    Part88B28 *self = object;

    if (object) {
        self->vtable = D_08A88B28;
        func_08A87328(self);
        func_08A8926C(&self->field_648, 2);
        func_08A88BC4(&self->field_62C, 2);
        func_08A89E34(&self->field_61C, 2);
        func_08A89814(&self->field_600, 2);
        func_08A90734(&self->field_5B0, 2);
        func_08A8F07C(&self->field_590, 2);
        func_08A8F7E4(&self->field_550, 2);
        func_08A923D4(&self->field_530, 2);
        func_08A915AC(&self->field_510, 2);
        func_08A91EC4(&self->field_4E0, 2);
        func_08A91AF4(&self->field_4C0, 2);
        func_08A90F0C(&self->field_4A0, 2);
        func_08A92934(&self->field_470, 2);
        func_08A900B4(&self->field_450, 2);
        func_08A8A3AC(&self->field_230, 2);
        func_08A95718(&self->field_130, 2);
        func_08A95718(&self->field_40, 2);
        func_08A7E778(&self->field_2C, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
