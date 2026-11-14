#include "battle/auto_region5.h"

/* Part54330: creates field_1970 from D_089D6AC8's and its own field_1974
 * data. */
void func_08B53254(Part54330 *self)
{
    self->field_1970 = func_08934944(GLOBAL_9D6AC8->field_710->field_04,
                                     D_08B54330->field_1974->field_08,
                                     D_08B54330->field_1974->field_04, 0x54, D_08B53248);
}
