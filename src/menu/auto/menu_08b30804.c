#include "menu/auto_region4.h"

/* Obj2F8F4: copies its five rows into block->rows[i].field_04. */
void func_08B30804(Obj2F8F4 *self, Block62C *block)
{
    int i;

    for (i = 0; i < 5; i++) {
        block->rows[i].field_04 = self->work->rows[i].field_00;
    }
}
