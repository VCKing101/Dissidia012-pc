#include "main/auto_region2.h"

/* Sets field_04, then func_08871938. */
void func_08871394(Obj9C9680 *self, int value)
{
    self->field_04 = value;
    func_08871938(self);
}
