#include "battle/auto_region5.h"

/* Part41AD0: loads field_08 from the shared D_08B422A8 (if loaded) and sets
 * field_00. */
void func_08B41AD0(Part41AD0 *self, unsigned char value)
{
    if (D_08B422A8 != 0) {
        func_08B41638(&self->field_08, D_08B422A8);
        self->field_01 = 1;
    }
    self->field_00 = value;
    self->field_3C = 0;
}
