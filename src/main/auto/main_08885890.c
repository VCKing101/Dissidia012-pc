#include "main/auto_region2.h"

/* func_08886520 on field_0C and func_088847E4 on field_10, where present. */
void func_08885890(Obj885220 *self, unsigned char arg)
{
    if (self->field_0C != 0) {
        func_08886520(self->field_0C, arg);
    }
    if (self->field_10 != 0) {
        func_088847E4(self->field_10, arg);
    }
}
