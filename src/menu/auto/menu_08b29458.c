#include "menu/auto_region4.h"

extern unsigned char D_08B29C44[];

/* ScreenB29CF8: fills its prompt block. */
void func_08B29458(ScreenB29CF8 *self)
{
    self->work->prompt.field_00 = 0;
    self->work->prompt.field_04 = -1;
    self->work->prompt.field_08 = -1;
    self->work->prompt.field_0C = self->work->field_580;
    self->work->prompt.field_10 = self->work->field_980;
    self->work->prompt.field_14 = self->work->texts;
    self->work->prompt.field_18 = 0;
    self->work->prompt.field_1C = self->work->messages;
    self->work->prompt.field_20 = 6;
    self->work->prompt.field_24 = D_08B29C44;
    self->work->prompt.field_28 = 0xFFFF;
    self->work->prompt.field_2A = 0xFFFF;
}
