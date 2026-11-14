#include "battle/auto_region5.h"

/* Part06984: clears the selection (field_9C, field_A0) and resets two
 * members of D_089D6A10. */
void func_08B0825C(Part06984 *self)
{
    func_08B0C5F4(self, -1);
    self->field_9C = 0;
    self->field_A0 = -1;
    func_088CDA34(GLOBAL_9D6A10->field_6C);
    func_088CE72C(GLOBAL_9D6A10->field_728);
}
