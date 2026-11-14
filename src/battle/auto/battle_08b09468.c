#include "battle/auto_region5.h"

/* Part06984: selects entry arg1 in D_089D6A10 and remembers arg2. */
int func_08B09468(Part06984 *self, int arg1, int arg2)
{
    func_088DE6D4(GLOBAL_9D6A10->field_E8, GLOBAL_9D6A10->field_38->field_5A0, arg1, 1);
    func_088CDB04(GLOBAL_9D6A10->field_6C);
    self->field_9C = 1;
    self->field_A0 = arg2;
    return 0;
}
