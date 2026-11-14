#include "menu/auto_region1.h"

/* Screen0B6A0: the largest func_088CBAD0 value among the records in
 * field_2E84 (at least 1). */
int func_089FC378(Screen0B6A0 *self)
{
    Record8CB164 *record;
    int id;
    int value;
    int i;
    int best;

    best = 1;
    for (i = 0; i < 5; i++) {
        id = self->work->field_2E84[i];
        if (id != 0) {
            record = func_088CB164(D_089D712C->field_F58, id);
            value = func_088CBAD0(record);
            if (best < value) {
                best = func_088CBAD0(record);
            }
        }
    }
    return best;
}
