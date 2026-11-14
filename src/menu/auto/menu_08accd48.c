#include "menu/auto_region3.h"

extern const short D_08AD91AC[];
extern const unsigned short D_08ADA14C[];

/* ScreenDB30: stops handle 0x3C and plays the closing scripts. */
void func_08ACCD48(ScreenDB30 *self)
{
    func_088D9F5C(self->work->handles[0x3C], -1, 1);
    func_088D442C(self->work->handles, self->work->texts, D_08ADA14C);
    func_088D3C24(&self->work->layout, self->work->handles, D_08AD91AC, 0x120);
}
