#include "menu/auto_region3.h"

extern const short D_08AB2F70[];
extern const unsigned short D_08AB2FD0[];

/* PanelAB2D24: plays its layout and text scripts. */
void func_08AB2D24(PanelAB2D24 *self)
{
    func_088D3F00(self->work->layout, self->work->handles, D_08AB2F70);
    func_088D44EC(self->work->handles, self->work->texts, D_08AB2FD0);
    func_08AB2B5C(self);
}
