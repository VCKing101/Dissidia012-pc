#include "battle/auto_region5.h"

/* Part47358: in phase 1, runs func_088D3F00 and func_088D44EC over the
 * handles and models. */
void func_08B475E4(Part47358 *self)
{
    if (self->phase == 1) {
        func_088D3F00(0, self->handles, self->field_2C);
        func_088D44EC(self->handles, self->models, D_08B48490);
    }
}
