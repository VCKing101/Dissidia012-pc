#include "menu/auto_region2.h"

/* Panel5B608: opens on record (cursor over 9 entries) and lays itself out. */
void func_08A59F18(Panel5B608 *self, Rec5B608 *record, int arg)
{
    unsigned int value;

    self->record = record;
    value = func_088729A0(0);
    self->field_1A08 = value;
    func_088D46E4(&self->cursor, 0, 0, 8, 0, -1);
    func_08A5A47C(self, arg);
    value = func_08A5A82C(self, self->record->field_08, self->record->field_0C);
    self->field_28 = value;
    func_08A5B060(self);
    func_08A5B38C(self);
    func_08A5B220(self);
    func_08A5A74C(self);
}
