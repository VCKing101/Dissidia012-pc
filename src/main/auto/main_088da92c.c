#include "main/auto_region4.h"

/* Obj8DA8E8: reset. */
void func_088DA92C(Obj8DA8E8 *self)
{
    self->callback = 0;
    self->callback_arg = 0;
    self->field_0C.object = 0;
    self->field_10.object = 0;
    self->flags = 1;
    self->field_16 = 0;
    self->field_15 = 0x96;
}
