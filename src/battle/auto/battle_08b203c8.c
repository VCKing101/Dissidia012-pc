#include "battle/auto_region5.h"

/* Part20A68: selects entry field_08 in field_18 and moves to phase 2. */
int func_08B203C8(Part20A68 *self)
{
    func_088DE6D4(self->field_18, self->field_08, 3, 1);
    self->phase = 2;
    return 0;
}
