#include "battle/auto_region4.h"

/* Part86940: stores the position and value and sets field_0C. */
int func_08A85F80(Part86940 *self, const BattleVec4 *position, int value)
{
    func_089A3868(&self->field_10, position);
    self->field_20 = value;
    self->field_0C = 1;
    return 0;
}
