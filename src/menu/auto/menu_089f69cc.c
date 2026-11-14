#include "menu/auto_region1.h"

extern const short D_08A094DE[];

/* Screen0B6A0: plays the release script D_08A094DE on all handles. */
void func_089F69CC(Screen0B6A0 *self)
{
    func_088D3C24(&self->work->layout, self->work->handles, D_08A094DE, 0x180);
}
