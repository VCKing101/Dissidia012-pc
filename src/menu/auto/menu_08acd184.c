#include "menu/auto_region3.h"

extern const unsigned short D_08ADA1CA[];
extern const unsigned short D_08ADA1EA[];

/* ScreenDB30: with flag 0x1000, plays D_08ADA1CA's closing form and clears the flag. */
void func_08ACD184(ScreenDB30 *self)
{
    WorkDB30 *work;

    work = self->work;
    if ((work->flags & 0x1000) != 0) {
        func_088D3EA0(&work->layout, work->handles, D_08ADA1CA);
        func_088D442C(self->work->handles, self->work->texts, D_08ADA1EA);
        self->work->flags = self->work->flags & 0xFFFFEFFF;
    }
}
