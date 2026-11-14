#include "menu/auto_region3.h"

extern const short D_08AD9614[];
extern const short D_08AD962C[];
extern const unsigned short D_08AD9664[];

/* ScreenDB30: plays D_08AD9614 / D_08AD962C and their texts. */
void func_08ACA704(ScreenDB30 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08AD9614);
    func_088D3F00(&self->work->layout, self->work->handles, D_08AD962C);
    func_088D44EC(self->work->handles, self->work->texts, D_08AD9664);
}
