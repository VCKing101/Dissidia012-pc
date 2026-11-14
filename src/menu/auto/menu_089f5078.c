#include "menu/auto_region1.h"

extern const short D_08A09BBA[];
extern const unsigned short D_08A09BF2[];
extern const short D_08A0A12A[];

/* Screen0B6A0: plays D_08A09BBA, the text script D_08A09BF2 and D_08A0A12A. */
void func_089F5078(Screen0B6A0 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08A09BBA);
    func_088D44EC(self->work->handles, self->work->texts, D_08A09BF2);
    func_088D3F00(&self->work->layout, self->work->handles, D_08A0A12A);
}
