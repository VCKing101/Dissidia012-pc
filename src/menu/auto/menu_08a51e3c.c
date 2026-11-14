#include "menu/auto_region2.h"

/* Panel base reset. */
void func_08A51E3C(Panel *self)
{
    self->field_1C = 0;
    self->field_20 = 0;
    self->field_00 = 0;
    self->handles = 0;
    self->texts = 0;
    self->field_0C = 0;
    self->handle_count = 0;
    self->text_count = 0;
    self->field_18 = 0;
}
