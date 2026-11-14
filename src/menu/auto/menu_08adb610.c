#include "menu/auto_region3.h"

extern const short D_08ADF78A[];
extern const unsigned short D_08ADF7CC[];

/* Screen46AC: plays D_08ADF78A and its texts. */
void func_08ADB610(Screen46AC *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08ADF78A);
    func_088D44EC(self->work->handles, self->work->texts, D_08ADF7CC);
}
