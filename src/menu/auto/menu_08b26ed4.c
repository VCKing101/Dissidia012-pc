#include "menu/auto_region4.h"

extern unsigned char D_08B27F88[];

/* Panel26C88: fills its prompt block. */
void func_08B26ED4(Panel26C88 *self)
{
    self->work->prompt.field_00 = 0;
    self->work->prompt.field_04 = -1;
    self->work->prompt.field_08 = -1;
    self->work->prompt.field_0C = self->work->layout->entries[10];
    self->work->prompt.field_10 = self->work->field_D4;
    self->work->prompt.field_14 = self->work->texts;
    self->work->prompt.field_18 = 0;
    self->work->prompt.field_1C = self->work->messages;
    self->work->prompt.field_20 = 0;
    self->work->prompt.field_24 = D_08B27F88;
    self->work->prompt.field_28 = 0xFFFF;
    self->work->prompt.field_2A = 0xFFFF;
}
