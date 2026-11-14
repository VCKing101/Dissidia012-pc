#include "menu/auto_region2.h"

/* Panel58420: hands its arrays and their lengths to the panel base. */
void func_08A575A4(Panel58420 *self)
{
    self->base.field_00 = self->field_34;
    self->base.handles = self->handles;
    self->base.texts = self->texts;
    self->base.field_0C = 0x27;
    self->base.handle_count = 5;
    self->base.text_count = 1;
}
