#include "battle/auto_region5.h"

/* Part49220: plays motions 7 and 0 on handles 2 and 3, shows handle 0 and
 * hides handle 1. */
void func_08B495DC(Part49220 *self)
{
    func_088D9F5C(self->handles[2], 7, 0);
    func_088D9F5C(self->handles[3], 0, 0);
    func_088D9CD8(self->handles[0]);
    func_088D9CFC(self->handles[1]);
}
