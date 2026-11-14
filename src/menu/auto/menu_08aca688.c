#include "menu/auto_region3.h"

extern const unsigned short D_08AD9614[];
extern const unsigned short D_08AD962C[];
extern const unsigned short D_08AD9664[];

/* ScreenDB30: plays D_08AD9614 / D_08AD962C and their texts (looped form). */
void func_08ACA688(ScreenDB30 *self)
{
    func_088D3CF0(&self->work->layout, self->work->handles, D_08AD9614);
    func_088D3CF0(&self->work->layout, self->work->handles, D_08AD962C);
    func_088D428C(self->work->handles, self->work->texts, D_08AD9664, 1);
    func_08ACA564(self);
}
