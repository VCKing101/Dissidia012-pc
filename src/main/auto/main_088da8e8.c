#include "main/auto_region4.h"

/* Obj8DA8E8: takes a source and copies its two size words as shorts. */
void func_088DA8E8(Obj8DA8E8 *self, Src8DA8E8 *source)
{
    self->callback = 0;
    self->callback_arg = 0;
    self->flags = 1;
    self->field_16 = 0;
    self->field_15 = 0x96;
    self->field_0C.object = source;
    if (source != 0) {
        self->field_18 = (short)source->field_24;
        self->field_1A = (short)source->field_2C;
    }
}
