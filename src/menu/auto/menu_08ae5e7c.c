#include "menu/auto_region3.h"

extern const unsigned short D_08AEB562[];
extern const unsigned short D_08AEB5B0[];

/* ScreenA690: plays D_08AEB562 and its texts (looped form). */
void func_08AE5E7C(ScreenA690 *self)
{
    func_088D3CF0(&self->work->layout, self->work->handles, D_08AEB562);
    func_088D428C(self->work->handles, self->work->texts, D_08AEB5B0, 1);
}
