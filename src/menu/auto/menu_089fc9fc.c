#include "menu/auto_region1.h"

extern const short D_08A0A9CA[];

/* Screen0B6A0: plays the layout script D_08A0A9CA. */
void func_089FC9FC(Screen0B6A0 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08A0A9CA);
}
