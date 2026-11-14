#include "menu/auto_region1.h"

/* Screen392E8: func_088DB10C on the 0x20 handles of field_08. */
void func_08A39330(Screen392E8 *self)
{
    int i;

    for (i = 0; i < 0x20; i++) {
        func_088DB10C(self->work->field_08->field_4C[i]);
    }
}
