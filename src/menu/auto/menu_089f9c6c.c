#include "menu/auto_region1.h"

extern const unsigned short D_08A0A1CA[];

/* Screen0B6A0: plays D_08A0A1CA, func_088D4BF4(0x27), then state 0xD. */
void func_089F9C6C(Screen0B6A0 *self)
{
    func_088D3CF0(&self->work->layout, self->work->handles, D_08A0A1CA);
    func_088D4BF4(0x27);
    self->work->state = 0xD;
}
