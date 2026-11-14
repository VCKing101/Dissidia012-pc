#include "battle/auto_region5.h"

/* Part148B4: plays motions 2, 8 and 2 on field_0C, field_10 and field_30
 * (the ones that exist). */
void func_08B148B4(Part148B4 *self)
{
    if (self->field_0C != 0) {
        func_088D9F5C(self->field_0C, 2, 1);
    }
    if (self->field_10 != 0) {
        func_088D9F5C(self->field_10, 8, 1);
    }
    if (self->field_30 != 0) {
        func_088D9F5C(self->field_30, 2, 1);
    }
}
