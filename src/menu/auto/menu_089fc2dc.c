#include "menu/auto_region1.h"

/* Screen0B6A0: the largest field_01 among the records in field_2E84 (at
 * least 1). */
unsigned char func_089FC2DC(Screen0B6A0 *self)
{
    Record8CB164 *record;
    int id;
    int i;
    unsigned char best;

    best = 1;
    for (i = 0; i < 5; i++) {
        id = self->work->field_2E84[i];
        if (id != 0) {
            record = func_088CB164(D_089D712C->field_F58, id);
            if (best < record->field_01) {
                best = record->field_01;
            }
        }
    }
    return best;
}
