#include "menu/auto_region3.h"

extern unsigned char D_08B016F4[];

/* ScreenB01CF8: fills its prompt block. */
void func_08AF6D3C(ScreenB01CF8 *self, void *entry, void *model, int arg)
{
    self->work->prompt.field_00 = 0;
    self->work->prompt.field_04 = -1;
    self->work->prompt.field_08 = -1;
    self->work->prompt.field_0C = entry;
    self->work->prompt.field_10 = model;
    self->work->prompt.field_14 = self->work->texts;
    self->work->prompt.field_18 = 0;
    self->work->prompt.field_1C = self->work->messages;
    self->work->prompt.field_20 = arg;
    self->work->prompt.field_24 = D_08B016F4;
    self->work->prompt.field_28 = 0xFFFF;
    self->work->prompt.field_2A = 0xFFFF;
}
