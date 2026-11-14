#include "menu/auto_region3.h"

extern const short D_08AB0070[];
extern const unsigned short D_08AB0620[];
extern const unsigned short D_08AB0678[];

/* ScreenAB7CFC: plays its layout and text scripts. */
void func_08AA9594(ScreenAB7CFC *self)
{
    func_088D3F00(self->work->layout, self->work->handles, D_08AB0070);
    func_088D44EC(self->work->handles, self->work->texts, D_08AB0620);
    func_088D44EC(self->work->handles, self->work->texts, D_08AB0678);
    func_08AAC5D0(self);
}
