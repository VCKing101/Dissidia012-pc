#include "menu/auto_region3.h"

extern const unsigned short D_08AD9614[];
extern const unsigned short D_08AD9664[];

/* ScreenDB30: plays D_08AD9614 and its texts (func_088D3EA0 / func_088D436C). */
void func_08ACA768(ScreenDB30 *self)
{
    func_088D3EA0(&self->work->layout, self->work->handles, D_08AD9614);
    func_088D436C(self->work->handles, self->work->texts, D_08AD9664);
}
