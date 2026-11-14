#include "main/auto_region2.h"

/* Stores the three values (-1 for arg2 becomes 0), then func_08889694 with arg4. */
void func_0888964C(Flow9D6A10 *self, unsigned int arg1, int arg2, unsigned int arg3, unsigned char arg4)
{
    self->field_4C = arg1;
    self->field_54 = arg2;
    if (arg2 == -1) {
        self->field_54 = 0;
    }
    self->field_740 = arg3;
    func_08889694(self, arg4);
}
