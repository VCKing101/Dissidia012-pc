#include "menu/auto_region1.h"

extern const short D_08A18350[];

/* Screen181E0: plays D_08A18350, then func_08A1609C and func_08A16384. */
void func_08A165F8(Screen181E0 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08A18350);
    func_08A1609C(self);
    func_08A16384(self);
}
