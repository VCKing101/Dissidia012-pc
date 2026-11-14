#include "menu/auto_region4.h"

/* Obj2F8F4: the picked row takes the picked id. */
void func_08B304F8(Obj2F8F4 *self)
{
    Work2F8F4 *work;

    work = self->work;
    work->rows[work->field_8EC].field_00 = work->field_990[work->field_938 * 2 + work->field_964];
}
