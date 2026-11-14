#include "menu/auto_region1.h"

extern const unsigned short D_08A3571C[];
extern const unsigned short D_08A356C4[];

/* Screen35550: sets field_6D3, plays two scripts, lays out field_120 and
 * calls func_08A2F134. */
void func_08A2F3B8(Screen35550 *self)
{
    self->work->field_6D3 = 1;
    func_088D3CF0(self->work->layout, self->work->handles, D_08A3571C);
    func_088D3CF0(self->work->layout, self->work->handles, D_08A356C4);
    func_08A41454(D_08A457EC, &self->work->field_120);
    func_08A2F134(self);
}
