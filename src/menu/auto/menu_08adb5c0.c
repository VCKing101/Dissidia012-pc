#include "menu/auto_region3.h"

extern const unsigned short D_08ADF78A[];
extern const unsigned short D_08ADF7CC[];

/* Screen46AC: plays D_08ADF78A and its texts (looped form). */
void func_08ADB5C0(Screen46AC *self)
{
    func_088D3CF0(&self->work->layout, self->work->handles, D_08ADF78A);
    func_088D428C(self->work->handles, self->work->texts, D_08ADF7CC, 1);
}
