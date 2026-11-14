#include "menu/auto_region3.h"

extern const unsigned short D_08AD9264[];
extern const unsigned short D_08AD932C[];

/* ScreenDB30: plays D_08AD9264 and its texts. */
void func_08AD062C(ScreenDB30 *self)
{
    func_08AD0240(self);
    func_088D3CF0(&self->work->layout, self->work->handles, D_08AD9264);
    func_088D428C(self->work->handles, self->work->texts, D_08AD932C, 1);
}
