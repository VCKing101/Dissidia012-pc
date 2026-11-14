#include "menu/auto_region3.h"

/* View8E3B4 deleting destructor. */
void func_08A8EC28(View8E3B4 *self, unsigned int flags)
{
    if (self != 0) {
        func_08A8B894(self);
        func_08A96D20(&self->field_18, 2);
        func_08A946F8(&self->field_0C, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
