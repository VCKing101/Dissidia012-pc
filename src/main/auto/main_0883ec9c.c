#include "main/auto_region2.h"

/* func_088A9BF8 on field_28; 1 without one. */
int func_0883EC9C(Obj83ECC4 *self)
{
    int result;

    result = 1;
    if (self->field_28 != 0) {
        result = func_088A9BF8(self->field_28);
    }
    return result;
}
