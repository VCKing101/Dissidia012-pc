#include "battle/auto_region5.h"

/* Part49AAC: stops field_5DC when `id` is the one running (or negative). */
void func_08B49F58(Part49AAC *self, int id)
{
    if (id == self->field_6EC || id < 0) {
        func_08932E48(self->field_5DC);
        self->field_6EC = -1;
    }
}
