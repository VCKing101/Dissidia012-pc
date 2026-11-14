#include "menu/auto_region3.h"

/* Label7259C: plays the close motion on node field_0C if it is shown. */
void func_08A72760(Label7259C *self)
{
    if (self->state == 1) {
        func_088D2F68(&self->field_0C, 4);
        self->state = 0;
    }
}
