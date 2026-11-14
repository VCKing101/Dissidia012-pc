#include "menu/auto_region2.h"

/* Panel694A8: plays its leave motions, state 3. */
void func_08A68544(Panel694A8 *self)
{
    func_088D9F5C(self->handles[1], 9, 1);
    func_088D9F5C(self->handles[5], 6, 1);
    func_088D9F5C(self->handles[0xB], 3, 1);
    func_08A68C34(self);
    self->state = 3;
}
