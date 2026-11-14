#include "menu/auto_region1.h"

/* Calls func_088FCA30 on D_089D7F90 when field_140 is 1. */
void func_089ECB40(ObjECB40 *self)
{
    if (self->field_140 == 1) {
        func_088FCA30(D_089D7F90);
    }
}
