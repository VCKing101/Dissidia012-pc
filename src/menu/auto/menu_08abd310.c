#include "menu/auto_region3.h"

extern const unsigned short D_08AC441A[];
extern const unsigned short D_08AC443A[];

/* ScreenC4560: plays D_08AC441A and its texts; state 0x1E. */
void func_08ABD310(ScreenC4560 *self)
{
    func_088D3CF0(&self->work->layout, self->work->handles, D_08AC441A);
    ((MenuModelView *)self->work->handles[0x38])->field_0E = 1;
    func_088D428C(self->work->handles, self->work->texts, D_08AC443A, 1);
    self->work->flags = self->work->flags | 0x20;
    self->work->state = 0x1E;
}
