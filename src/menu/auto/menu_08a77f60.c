#include "menu/auto_region3.h"

/* Step77C34: plays the leave motions; state 3, then the given state. */
void func_08A77F60(Step77C34 *self, int next)
{
    func_08A743E0(&self->field_04);
    func_08A72F5C(&self->field_14);
    self->state = 3;
    self->next = next;
}
