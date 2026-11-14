#include "battle/auto_region5.h"

/* Part49AAC: unless one is running, starts `id` on field_5DC. */
void func_08B49F08(Part49AAC *self, int id)
{
    if (self->field_6EC < 0) {
        self->field_6EC = id;
        func_08932C74(self->field_5DC, id, self->field_00, 4, 0, 2);
    }
}
