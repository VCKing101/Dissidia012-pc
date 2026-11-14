#include "menu/auto_region2.h"

extern const unsigned short D_08A48070[];

/* Screen47D90: hands its texts to its handles (script D_08A48070). */
void func_08A45C14(Screen47D90 *self, int arg)
{
    func_088D428C(self->work->handles, self->work->texts, D_08A48070, 1);
}
