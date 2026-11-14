#include "battle/auto_region5.h"

/* Part3AF54: sets field_11C and runs func_08B4024C on field_148. */
void func_08B3B5EC(Part3AF54 *self, int value)
{
    self->field_11C = value;
    func_08B4024C(self->field_148);
}
