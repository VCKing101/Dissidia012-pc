#include "menu/auto_region1.h"

extern const short D_08A15B78[];
extern const short D_08A15DB8[];
extern const short D_08A15E48[];
extern const unsigned short D_08A15BCA[];

/* Screen15A88: plays three layout scripts and the text script D_08A15BCA. */
void func_08A1313C(Screen15A88 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08A15B78);
    func_088D3F00(&self->work->layout, self->work->handles, D_08A15DB8);
    func_088D3F00(&self->work->layout, self->work->handles, D_08A15E48);
    func_088D44EC(self->work->handles, self->work->texts, D_08A15BCA);
}
