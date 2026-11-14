#include "menu/auto_region3.h"

/* Step77C34: plays the leave motions when value is 0; state 2. */
void func_08A78228(Step77C34 *self, int arg, int value)
{
    if (value == 0) {
        func_08A743E0(&self->field_04);
        func_08A72F5C(&self->field_14);
        self->state = 2;
    }
}
