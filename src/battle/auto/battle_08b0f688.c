#include "battle/auto_region5.h"

/* Part0F410: sets flag 1 on its object and runs func_08A80EBC on it. */
void func_08B0F688(Part0F410 *self)
{
    void *object;

    object = func_08B11718(self->field_30);
    if (object != 0) {
        func_08A80B90(object, 1);
        func_08A80EBC(object);
    }
}
