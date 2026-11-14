#include "menu/auto_region4.h"

extern unsigned char D_08B32CBC[];

/* ScreenB32D90: fills its prompt block (with the messages of ScreenB01CF8). */
void func_08B32530(ScreenB32D90 *self)
{
    self->work->prompt.field_00 = 0;
    self->work->prompt.field_04 = -1;
    self->work->prompt.field_08 = -1;
    self->work->prompt.field_0C = self->work->field_574;
    self->work->prompt.field_10 = self->work->field_974;
    self->work->prompt.field_14 = self->work->texts;
    self->work->prompt.field_18 = 0;
    self->work->prompt.field_1C = ((ScreenB01CF8X *)D_08B01CF8)->work->messages;
    self->work->prompt.field_20 = 6;
    self->work->prompt.field_24 = D_08B32CBC;
    self->work->prompt.field_28 = 0xFFFF;
    self->work->prompt.field_2A = 0xFFFF;
}
