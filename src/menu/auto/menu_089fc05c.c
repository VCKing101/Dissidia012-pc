#include "menu/auto_region1.h"

extern const short D_08A0A7B6[];

/* Screen0B6A0: plays the release script D_08A0A7B6 on the second handle set,
 * then func_089FCFE0. */
void func_089FC05C(Screen0B6A0 *self)
{
    func_088D3C24(&self->work->layout, self->work->handles2, D_08A0A7B6, 0x40);
    func_089FCFE0(self);
}
