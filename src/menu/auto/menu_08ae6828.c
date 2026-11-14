#include "menu/auto_region3.h"

extern const short D_08AEB3E4[];
extern const unsigned short D_08AEBA50[];
extern const unsigned short D_08AEBAC0[];

/* ScreenA690: stops handle 0x66 and plays the closing scripts. */
void func_08AE6828(ScreenA690 *self)
{
    func_088D9F5C(self->work->handles[0x66], -1, 1);
    func_088D442C(self->work->handles, self->work->texts, D_08AEBA50);
    func_088D442C(self->work->handles, self->work->texts, D_08AEBAC0);
    func_088D3C24(&self->work->layout, self->work->handles, D_08AEB3E4, 0x120);
}
