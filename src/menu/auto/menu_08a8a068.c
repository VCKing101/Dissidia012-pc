#include "menu/auto_region3.h"

/* Screen8B7A8: follows the mode of field_158's entry (2 and 4 switch field_154). */
void func_08A8A068(Screen8B7A8 *self)
{
    int mode;
    Work8B7A8 *work;

    if (((self->field_158 != 0) &&
         (mode = func_08B2BB8C(self->field_158->ref->id), mode != -1)) &&
        (work = self->work, mode != work->mode)) {
        if (mode == 2) {
            func_08A8EF2C(self->field_154, 1, 4);
            work = self->work;
        }
        else if (mode == 4) {
            func_08A8EF2C(self->field_154, 0, 0);
            work = self->work;
        }
        work->mode = mode;
    }
}
