#include "battle/auto_region3.h"

void func_088D9F5C(void *handle, int arg1, char arg2);

/* Calls func_088D9F5C(handle, 0xC, 0) on the first handle at field_228. */
void func_08A3DCA8(Model3E874 *self)
{
    if (self->field_228[0] != 0) {
        func_088D9F5C(self->field_228[0], 0xC, 0);
    }
}
