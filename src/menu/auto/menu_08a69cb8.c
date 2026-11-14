#include "menu/auto_region2.h"

/* Panel6B170 reset. */
void func_08A69CB8(Panel6B170 *self)
{
    int i;

    func_08A51E3C(&self->base);
    self->state = 0;
    self->field_2C = 0;
    for (i = 0; i < 9; i++) {
        self->field_594[i] = 0;
    }
    for (i = 0; i < 0x1C; i++) {
        self->handles[i] = 0;
    }
    for (i = 0; i < 1; i++) {
        self->texts[i] = 0;
    }
    self->field_66C = 0;
    self->field_30 = 0;
    func_088D38F0(&self->layout);
    self->field_478 = 0;
    self->field_434 = 0;
    self->field_670 = -1;
    self->field_674 = 0;
    self->field_678 = -1;
}
