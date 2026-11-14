#include "menu/auto_region1.h"

extern unsigned char D_089E744C[];

/* ScreenE75C4: fills its prompt block. */
void func_089E2FB8(ScreenE75C4 *self)
{
    self->work->prompt.field_00 = 0;
    self->work->prompt.field_04 = -1;
    self->work->prompt.field_08 = -1;
    self->work->prompt.field_0C = self->work->layout.entries[0xC];
    self->work->prompt.field_10 = self->work->handles[0x44];
    self->work->prompt.field_14 = &self->work->texts[0x52];
    self->work->prompt.field_18 = 0;
    self->work->prompt.field_1C = self->work->messages;
    self->work->prompt.field_20 = 0;
    self->work->prompt.field_24 = D_089E744C;
    self->work->prompt.field_28 = 0xFFFF;
    self->work->prompt.field_2A = 0xFFFF;
}
