#include "battle/auto_region4.h"

/* PartB32A8 deleting destructor: destroys the members in reverse order. */
void func_08AB2F60(void *object, int flags)
{
    PartB32A8 *self = object;

    if (object) {
        self->vtable = D_08AB32A8;
        func_08AB3014(self);
        func_08A86CE0(&self->field_440, 2);
        func_08A7E990(&self->field_390, 2);
        func_08A7B208(&self->field_36C, 2);
        func_08A64EE0(&self->field_360, 2);
        func_08AAE1C0(&self->field_34C, 2);
        func_08A82420(&self->field_198, 2);
        func_08A97150(&self->field_04, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
