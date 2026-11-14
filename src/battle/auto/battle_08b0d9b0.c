#include "battle/auto_region5.h"

/* Part0D9B0: runs func_0889F830 on field_04's member, if any. */
void func_08B0D9B0(Part0D9B0 *self)
{
    if (self->field_04 != 0) {
        func_0889F830(self->field_04->field_04, 0);
    }
}
