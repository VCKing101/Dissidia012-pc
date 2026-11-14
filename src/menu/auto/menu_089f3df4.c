#include "menu/auto_region1.h"

extern const unsigned short D_08A09B2A[];

/* Screen0B6A0: plays the text script D_08A09B2A. */
void func_089F3DF4(Screen0B6A0 *self)
{
    func_088D44EC(self->work->handles, self->work->texts, D_08A09B2A);
}
