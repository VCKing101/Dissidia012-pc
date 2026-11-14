#include "menu/auto_region1.h"

/* Screen38EA8: makes texts 0..3 and 0x14 through D_08A457EC, then
 * func_08A37AC8 for 0 and 1. */
void func_08A35C04(Screen38EA8 *self)
{
    self->work->texts[0] = func_08A412A8(D_08A457EC, 0x2E, 0, 0);
    self->work->texts[1] = func_0887E534(self->work->texts[0]);
    self->work->texts[2] = func_08A412A8(D_08A457EC, 0x2F, 0, 0);
    self->work->texts[3] = func_0887E534(self->work->texts[2]);
    self->work->texts[0x14] = func_08A412A8(D_08A457EC, 0x11, 0, 0);
    func_08A37AC8(self, 0);
    func_08A37AC8(self, 1);
}
