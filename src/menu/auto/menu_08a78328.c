#include "menu/auto_region3.h"

/* Step77C34: plays the enter motions; state 1. */
void func_08A78328(Step77C34 *self)
{
    func_08A74390(&self->field_04);
    func_08A72ED0(&self->field_14);
    self->state = 1;
    self->next = 0;
}
