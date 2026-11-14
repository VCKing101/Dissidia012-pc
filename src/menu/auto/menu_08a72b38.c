#include "menu/auto_region3.h"

/* Stage72ABC: plays the leave motions; state 2. */
void func_08A72B38(Stage72ABC *self)
{
    func_088D2F68(&self->field_08, 0x23);
    if (self->field_24 == 1) {
        func_088D2F68(&self->field_10, 5);
    }
    self->state = 2;
}
