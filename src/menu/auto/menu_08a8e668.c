#include "menu/auto_region3.h"

extern unsigned char D_08A93CE0[];

/* View8E3B4: fills its prompt block. */
void func_08A8E668(View8E3B4 *self)
{
    self->work->prompt.field_00 = 0;
    self->work->prompt.field_04 = -1;
    self->work->prompt.field_08 = -1;
    self->work->prompt.field_0C = self->work->layout->entries[0x44];
    self->work->prompt.field_10 = self->work->handles[0x85];
    self->work->prompt.field_14 = self->work->prompt_texts;
    self->work->prompt.field_18 = 0;
    self->work->prompt.field_1C = self->work->messages;
    self->work->prompt.field_20 = 2;
    self->work->prompt.field_24 = D_08A93CE0;
    self->work->prompt.field_28 = 0xFFFF;
    self->work->prompt.field_2A = 0xFFFF;
}
