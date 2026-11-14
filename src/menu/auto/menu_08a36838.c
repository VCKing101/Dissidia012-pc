#include "menu/auto_region1.h"

/* Screen38EA8: releases field_D60 through D_089D6C80 when set, clears
 * field_D5C. */
void func_08A36838(Screen38EA8 *self)
{
    Work38EA8 *work;

    work = self->work;
    if (work->field_D60 != -1) {
        func_088A0DFC(D_089D6C80, work->field_D60);
        self->work->field_D60 = -1;
        work = self->work;
    }
    work->field_D5C = -1;
}
