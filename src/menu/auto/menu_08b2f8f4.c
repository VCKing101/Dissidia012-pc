#include "menu/auto_region4.h"

extern const unsigned short D_08B31846[];

/* Obj2F8F4: marks each of the sixteen ids func_08B2F898 accepts. */
void func_08B2F8F4(Obj2F8F4 *self)
{
    int i;

    for (i = 0; i < 0x10; i++) {
        if (func_08B2F898(self, self->work->field_990[i]) != 0) {
            func_088D3AE8(self->work->handles, 0x2D, D_08B31846[i], i + 0x2F);
        }
    }
}
