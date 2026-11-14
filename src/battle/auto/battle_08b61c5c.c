#include "battle/auto_region5.h"

/* Part61C08: clears field_10 and field_0C and turns D_089D4DC8's field_7180
 * on. */
void func_08B61C5C(Part61C08 *self)
{
    self->field_10 = 0;
    self->field_0C = 0;
    func_0885BA0C(D_089D4DC8->field_7180, 1);
}
