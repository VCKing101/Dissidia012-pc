#include "menu/auto_region3.h"

/* ScreenA3F44: closes field_894 if open, then enters state 5. */
void func_08A9832C(ScreenA3F44 *self)
{
    if ((self->work->flags & 1) != 0) {
        func_08AB58E0(&self->field_894, 6, 0);
        func_08AB5790(&self->field_894, 2);
    }
    func_08A9C55C(self, 0);
    self->work->field_81C4 = 0;
    func_08AA0D48(self, 0);
    self->work->state = 5;
    func_08A9E0E8(self);
    func_08A9BEA0(self);
}
