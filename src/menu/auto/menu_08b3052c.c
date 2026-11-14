#include "menu/auto_region4.h"

/* Obj2F8F4: func_08B2F898 refuses the picked id. */
unsigned char func_08B3052C(Obj2F8F4 *self)
{
    Work2F8F4 *work;
    int accepted;

    work = self->work;
    accepted = func_08B2F898(self, work->field_990[work->field_938 * 2 + work->field_964]);
    return accepted == 0;
}
