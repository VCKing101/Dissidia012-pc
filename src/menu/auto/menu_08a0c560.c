#include "menu/auto_region1.h"

extern const short D_08A11708[];
extern const unsigned short D_08A11916[];

/* Screen11510: plays D_08A11708 and the text script D_08A11916, then
 * func_08A0C0EC. */
void func_08A0C560(Screen11510 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08A11708);
    func_088D44EC(self->work->handles, self->work->texts, D_08A11916);
    func_08A0C0EC(self);
}
