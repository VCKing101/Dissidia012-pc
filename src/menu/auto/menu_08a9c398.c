#include "menu/auto_region3.h"

extern unsigned char D_08AA3D8C[];

/* ScreenA3F44: fills its prompt block. */
void func_08A9C398(ScreenA3F44 *self)
{
    self->work->prompt.field_00 = 0;
    self->work->prompt.field_04 = -1;
    self->work->prompt.field_08 = -1;
    self->work->prompt.field_0C = self->work->field_8AF8;
    self->work->prompt.field_10 = self->work->field_8EF8;
    self->work->prompt.field_14 = self->work->texts;
    self->work->prompt.field_18 = 0;
    self->work->prompt.field_1C = self->work->messages;
    self->work->prompt.field_20 = 0;
    self->work->prompt.field_24 = D_08AA3D8C;
    self->work->prompt.field_28 = 0xFFFF;
    self->work->prompt.field_2A = 0xFFFF;
}
