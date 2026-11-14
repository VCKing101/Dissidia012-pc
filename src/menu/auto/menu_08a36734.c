#include "menu/auto_region1.h"

/* Screen38EA8: func_0893476C(out, func_08A3C0C4(6), field_D64); reports 100. */
int func_08A36734(Screen38EA8 *self, void *out)
{
    int value;

    value = func_08A3C0C4(6);
    func_0893476C(out, value, self->work->field_D64);
    return 100;
}
