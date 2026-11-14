#include "battle/auto_region5.h"

/* Part297C8 release: runs func_08B28FB4, releases field_04 and resets. */
void func_08B28F70(Part297C8 *self)
{
    func_08B28FB4(self);
    if (self->field_04 != 0) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_08B28F18(self);
}
