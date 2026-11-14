#include "menu/auto_region1.h"

extern const unsigned short D_08A096C2[];
extern const unsigned short D_08A097BA[];
extern const unsigned short D_08A09A4A[];

/* Screen0B6A0: plays three layout scripts, hides handles 0x80..0x88 and
 * calls func_089F77E0. */
void func_089F5A4C(Screen0B6A0 *self)
{
    func_088D3CF0(&self->work->layout, self->work->handles, D_08A096C2);
    func_088D3CF0(&self->work->layout, self->work->handles, D_08A097BA);
    func_088D3CF0(&self->work->layout, self->work->handles, D_08A09A4A);
    func_089F5990(self, 0x80);
    func_089F77E0(self, 0, 0, 1);
}
