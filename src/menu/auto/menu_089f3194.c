#include "menu/auto_region1.h"

/* Screen0B6A0: resets fields 3214..321A, then func_089F3498(1). */
void func_089F3194(Screen0B6A0 *self)
{
    self->work->field_3214 = 0;
    self->work->field_3218 = 1;
    self->work->field_321A = 0;
    func_089F3498(self, 1);
}
