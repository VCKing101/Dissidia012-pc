#include "menu/auto_region1.h"

extern const short D_08A09CB2[];
extern const unsigned short D_08A09CDA[];

/* Screen0B6A0: plays D_08A09CB2 and the text script D_08A09CDA. */
void func_089F5148(Screen0B6A0 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08A09CB2);
    func_088D44EC(self->work->handles, self->work->texts, D_08A09CDA);
}
