#include "menu/agent29_types.h"

/* Screen0B6A0: for each finished record 1-4 (done flag 1 of +0x2E98),
   takes `percent` of its points (+0x32C8) off the record's count, kept
   within 0 and the points, stores what is left to go (+0x32A0) and
   clears the done flag. */
void func_089FC590(Screen0B6A0 *self, unsigned int percent)
{
    int i;
    int total;
    int left;
    Record8CB164 *record;

    for (i = 1; i < 5; i++) {
        if (self->work->field_2E84[i] != 0 && ((&WORK_INT(self->work, 0x2E98))[i] & 1) != 0) {
            record = func_088CB164(D_089D712C->field_F58, self->work->field_2E84[i]);
            total = (&WORK_INT(self->work, 0x32C8))[i];
            left = record->field_0C - (int)(total * percent) / 100;
            if (left < 0) {
                left = 0;
            } else if (left > total) {
                left = total;
            }
            record->field_0C = left;
            (&WORK_INT(self->work, 0x32A0))[i] = (&WORK_INT(self->work, 0x32C8))[i] - left;
            (&WORK_INT(self->work, 0x2E98))[i] &= ~1;
        }
    }
}
