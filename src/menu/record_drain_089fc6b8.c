#include "menu/agent29_types.h"

/* Screen0B6A0: takes `percent` (at most 100, none with flag 0x10 on) of
   their points off the counts of the records 1-4 that are set and not
   done, kept within 0 and the points, and stores what is left to go
   (+0x32A0). */
void func_089FC6B8(Screen0B6A0 *self, int percent)
{
    int i;
    int total;
    int left;
    Record8CB164 *record;

    if (func_089EFB50(self, 0x10, -1) != 0) {
        percent = 0;
    } else if (percent > 100) {
        percent = 100;
    }
    for (i = 1; i < 5; i++) {
        if (self->work->field_2E84[i] != 0 && ((&WORK_INT(self->work, 0x2E98))[i] & 1) == 0) {
            record = func_088CB164(D_089D712C->field_F58, self->work->field_2E84[i]);
            total = (&WORK_INT(self->work, 0x32C8))[i];
            left = record->field_0C - (int)(total * (unsigned int)percent) / 100;
            if (left < 0) {
                left = 0;
            } else if (left > total) {
                left = total;
            }
            record->field_0C = left;
            (&WORK_INT(self->work, 0x32A0))[i] = (&WORK_INT(self->work, 0x32C8))[i] - left;
        }
    }
}
