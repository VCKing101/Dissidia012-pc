#include "battle/auto_region4.h"

/* PartF3D40: plays motion 2 on the handle at field_04, if any. */
void func_08AF3D40(PartF3D40 *self)
{
    if (self->field_04 != 0) {
        func_088D9F5C(self->field_04, 2, 1);
    }
}
