#include "menu/auto_region1.h"

/* ObjEE228: plays motion 2 on its third model, then state 2. */
void func_089EE5DC(ObjEE228 *self)
{
    func_088D9F5C(self->models[2], 2, 1);
    self->state = 2;
}
