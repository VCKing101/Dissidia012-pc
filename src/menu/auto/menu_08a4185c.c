#include "menu/auto_region2.h"

extern unsigned char D_08A456C0[];

/* MenuRoot2: fills its prompt for handle index. */
void func_08A4185C(MenuRoot2 *self, int index)
{
    self->work->prompt.field_00 = 0;
    self->work->prompt.field_04 = -1;
    self->work->prompt.field_08 = -1;
    self->work->prompt.field_0C = self->work->layout.entries[8];
    self->work->prompt.field_10 = self->work->handles[index];
    self->work->prompt.field_14 = self->work->texts;
    self->work->prompt.field_18 = 0;
    self->work->prompt.field_1C = self->work->messages;
    self->work->prompt.field_20 = 0;
    self->work->prompt.field_24 = D_08A456C0;
    self->work->prompt.field_28 = 0xFFFF;
    self->work->prompt.field_2A = 0xFFFF;
}
