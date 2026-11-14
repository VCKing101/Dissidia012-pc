#include "main/auto_region2.h"

/* Sets field_70, clears field_134 and registers func_08857108 with D_089D7830. */
void func_088569A4(Part85697C *self)
{
    int *handle;

    self->field_70 = 1;
    self->field_134 = 0;
    handle = func_088D107C(D_089D7830, func_08857108, 0);
    self->field_138 = handle;
}
