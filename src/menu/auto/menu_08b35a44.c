#include "menu/auto_region4.h"

/* ScreenB41CC0: clears the callbacks func_08B3CAF0 sets. */
void func_08B35A44(ScreenB41CC0 *self)
{
    int i;

    for (i = 0; i < 3; i++) {
        self->field_98[i] = 0;
    }
}
