#include "battle/auto_region4.h"

/* PartE7B50: in phase 1 or 4, calls func_089DDCEC and clears field_299;
 * then calls func_08AE1270 (phase 0). */
void func_08AE2148(PartE7B50 *self)
{
    if (self->phase == 1 || self->phase == 4) {
        func_089DDCEC(D_089D6A10->field_7A4);
        if (self->field_299 != 0) {
            self->field_299 = 0;
        }
    }
    func_08AE1270(self, 0);
}
