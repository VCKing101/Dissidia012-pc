#include "battle/auto_region5.h"

/* Part1D810 step: in phase 2 waits for field_64 (then phase 3); otherwise
 * runs field_68. */
int func_08B1D190(Part1D810 *self)
{
    if (self->phase == 2) {
        if (func_08B1A758(self->field_64) == 1) {
            self->phase = 3;
        }
    } else {
        func_08B1C6EC(self->field_68);
    }
    return 0;
}
