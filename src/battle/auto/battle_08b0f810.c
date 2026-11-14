#include "battle/auto_region5.h"

/* Part0F410: clears flag 0 on its object and runs func_08A81078 on it. */
void func_08B0F810(Part0F410 *self)
{
    void *object;

    object = func_08B11718(self->field_30);
    if (object != 0) {
        func_08A80B90(object, 0);
        func_08A81078(object);
    }
}
