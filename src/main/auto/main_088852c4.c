#include "main/auto_region2.h"

/* Passes arg to the parts (to field_10 only when skip is 0). */
void func_088852C4(Obj885220 *self, unsigned char skip, unsigned char arg)
{
    func_08884B00(self->field_04, arg, 0);
    func_08885E2C(self->field_08, arg, 0);
    func_08886288(self->field_0C, arg);
    if (skip == '\0') {
        func_08884388(self->field_10, arg, 0);
    }
}
