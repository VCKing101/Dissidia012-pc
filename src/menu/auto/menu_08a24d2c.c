#include "menu/auto_region1.h"

/* Screen28560: field_00 of the row that field_1FD4[field_2000] picks. */
int func_08A24D2C(Screen28560 *self)
{
    Work28560 *work;

    work = self->work;
    return work->field_1E9C[work->field_1FD4[work->field_2000]].field_00;
}
