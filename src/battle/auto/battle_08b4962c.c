#include "battle/auto_region5.h"

/* Part49220: plays motions 6 and 1 on handles 2 and 3, hides handle 0 and
 * shows handle 1. */
void func_08B4962C(Part49220 *self)
{
    func_088D9F5C(self->handles[2], 6, 0);
    func_088D9F5C(self->handles[3], 1, 0);
    func_088D9CFC(self->handles[0]);
    func_088D9CD8(self->handles[1]);
}
