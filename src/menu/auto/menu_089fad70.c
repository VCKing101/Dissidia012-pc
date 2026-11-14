#include "menu/auto_region1.h"

extern unsigned char D_08A0AEFC[];

/* Screen0B6A0: fills its prompt block. */
void func_089FAD70(Screen0B6A0 *self)
{
    self->work->prompt.field_00 = 0;
    self->work->prompt.field_04 = -1;
    self->work->prompt.field_08 = -1;
    self->work->prompt.field_0C = self->work->layout.entries[0x3A];
    self->work->prompt.field_10 = self->work->handles[0];
    self->work->prompt.field_14 = self->work->texts;
    self->work->prompt.field_18 = 0;
    self->work->prompt.field_1C = self->work->messages;
    self->work->prompt.field_20 = 0;
    self->work->prompt.field_24 = D_08A0AEFC;
    self->work->prompt.field_28 = 0xFFFF;
    self->work->prompt.field_2A = 0xFFFF;
}
