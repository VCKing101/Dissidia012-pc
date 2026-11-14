#include "menu/auto_region3.h"

extern const unsigned short D_08AEB562[];
extern const unsigned short D_08AEB5B0[];

/* ScreenA690: plays D_08AEB562 and its texts (func_088D3EA0 / func_088D436C). */
void func_08AE5F20(ScreenA690 *self)
{
    func_088D3EA0(&self->work->layout, self->work->handles, D_08AEB562);
    func_088D436C(self->work->handles, self->work->texts, D_08AEB5B0);
}
