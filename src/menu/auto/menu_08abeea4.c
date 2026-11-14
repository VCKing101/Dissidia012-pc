#include "menu/auto_region3.h"

extern const unsigned short D_08AC4396[];
extern const unsigned short D_08AC43B6[];

/* ScreenC4560: plays D_08AC4396 and its texts; state from func_08ABC868. */
void func_08ABEEA4(ScreenC4560 *self)
{
    int state;

    func_088D3CF0(&self->work->layout, self->work->handles, D_08AC4396);
    ((MenuModelView *)self->work->handles[0x38])->field_0E = 1;
    func_088D428C(self->work->handles, self->work->texts, D_08AC43B6, 1);
    self->work->flags = self->work->flags | 0x20;
    state = func_08ABC868(self);
    self->work->state = state;
}
