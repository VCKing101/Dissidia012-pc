#include "menu/auto_region1.h"

extern unsigned char D_08A23B30[];

/* Menu root: fills its prompt block. */
void func_08A1E070(MenuRoot *self, void *entry, void *handle, int value)
{
    self->work->prompt.field_00 = 0;
    self->work->prompt.field_04 = -1;
    self->work->prompt.field_08 = -1;
    self->work->prompt.field_0C = entry;
    self->work->prompt.field_10 = handle;
    self->work->prompt.field_14 = &self->work->field_8DC;
    self->work->prompt.field_18 = 0;
    self->work->prompt.field_1C = self->work->messages;
    self->work->prompt.field_20 = value;
    self->work->prompt.field_24 = D_08A23B30;
    self->work->prompt.field_28 = 0xFFFF;
    self->work->prompt.field_2A = 0xFFFF;
}
