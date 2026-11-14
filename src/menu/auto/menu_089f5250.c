#include "menu/auto_region1.h"

extern const short D_08A09D42[];
extern const unsigned short D_08A09D6A[];

/* Screen0B6A0: plays D_08A09D42 and the text script D_08A09D6A. */
void func_089F5250(Screen0B6A0 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08A09D42);
    func_088D44EC(self->work->handles, self->work->texts, D_08A09D6A);
}
