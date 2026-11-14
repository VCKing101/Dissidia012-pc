#include "menu/auto_region1.h"

extern const short D_08A09C62[];
extern const unsigned short D_08A09C9A[];
extern const short D_08A0A12A[];

/* Screen0B6A0: plays D_08A09C62, the text script D_08A09C9A and D_08A0A12A. */
void func_089F50E0(Screen0B6A0 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08A09C62);
    func_088D44EC(self->work->handles, self->work->texts, D_08A09C9A);
    func_088D3F00(&self->work->layout, self->work->handles, D_08A0A12A);
}
