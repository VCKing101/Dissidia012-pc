#include "menu/auto_region1.h"

extern const short D_08A15B78[];
extern const unsigned short D_08A15BCA[];

/* Screen15A88: plays D_08A15B78 and the text script D_08A15BCA, then
 * func_08A124FC(1). */
void func_08A12644(Screen15A88 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08A15B78);
    func_088D44EC(self->work->handles, self->work->texts, D_08A15BCA);
    func_08A124FC(self, 1);
}
