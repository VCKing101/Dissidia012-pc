#include "menu/auto_region4.h"

void func_08B4FE98(Elem4FE98 *self)
{
    int i;

    self->state = 0;
    for (i = 0; i < 5; i++) {
        self->handles[i] = 0;
    }
    self->model = 0;
    self->index = 0;
    self->field_20 = 0xB;
}
