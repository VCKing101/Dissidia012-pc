#include "menu/auto_region1.h"

extern const short D_08A09C22[];
extern const unsigned short D_08A09C3A[];

/* Screen0B6A0: plays D_08A09C22 and the text script D_08A09C3A. */
void func_089F5200(Screen0B6A0 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08A09C22);
    func_088D44EC(self->work->handles, self->work->texts, D_08A09C3A);
}
