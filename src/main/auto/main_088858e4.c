#include "main/auto_region2.h"

/* func_08886588 on field_0C and func_08884898 on field_10, where present. */
void func_088858E4(Obj885220 *self, unsigned char arg)
{
    if (self->field_0C != 0) {
        func_08886588(self->field_0C, arg);
    }
    if (self->field_10 != 0) {
        func_08884898(self->field_10, arg);
    }
}
