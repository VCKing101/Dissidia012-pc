#include "menu/auto_region3.h"

extern const short D_08AEB562[];
extern const unsigned short D_08AEB5B0[];

/* ScreenA690: plays D_08AEB562 and its texts. */
void func_08AE5ED0(ScreenA690 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08AEB562);
    func_088D44EC(self->work->handles, self->work->texts, D_08AEB5B0);
}
