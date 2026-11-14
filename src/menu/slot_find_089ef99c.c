#include "menu/agent29_types.h"

/* Screen0B6A0: the id in the first of the three slots (+0x2E70) whose
   kind (func_089EF880) is `kind` and that is not used yet; -1 when there
   is none. */
extern unsigned char func_089EF880(Screen0B6A0 *self, int id);

int func_089EF99C(Screen0B6A0 *self, unsigned char kind)
{
    int found = -1;
    int i;

    for (i = 0; i < 3; i++) {
        if (self->work->slots[i].field_00 == -1) {
            break;
        }
        if (func_089EF880(self, self->work->slots[i].field_00) == kind &&
            (self->work->slots[i].field_01 & 1) == 0) {
            found = self->work->slots[i].field_00;
            break;
        }
    }
    return found;
}
