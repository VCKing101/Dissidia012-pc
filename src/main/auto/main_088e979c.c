#include "main/auto_region4.h"

/* Obj9D7E88: copies the name (if any) and value, then sets the callbacks. */
void func_088E979C(Obj9D7E88 *self, const char *name, int value)
{
    func_08844588(D_089D4960);
    if (name != 0) {
        func_089359E8(name);
        func_089359A4(self->name, name);
    }
    self->field_1864 = value;
    self->field_1B06 = 0;
    func_088E8DC4(self);
}
