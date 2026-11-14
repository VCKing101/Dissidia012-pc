#include "menu/auto_region1.h"

/* ObjEE850: plays motion 0x23 on its sixth model, then state 2. */
void func_089EEB4C(ObjEE850 *self)
{
    func_088D9F5C(self->models[5], 0x23, 1);
    self->state = 2;
}
