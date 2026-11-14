#include "main/auto_region4.h"

/* Obj9D7E88: clears the name, stores the values, then sets the callbacks. */
void func_088E9848(Obj9D7E88 *self, unsigned int arg1, unsigned char arg2, unsigned int value)
{
    func_08844588(D_089D4960);
    self->name[0] = 0;
    self->field_7C = arg1;
    self->field_80 = arg2;
    self->field_1864 = value;
    self->field_1B06 = 0;
    func_088E8DC4(self);
}
