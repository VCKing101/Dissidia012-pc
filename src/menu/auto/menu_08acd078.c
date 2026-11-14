#include "menu/auto_region3.h"

extern const short D_08ADA1CA[];
extern const unsigned short D_08ADA1EA[];

/* ScreenDB30: with flag 0x1000, plays D_08ADA1CA and its texts. */
void func_08ACD078(ScreenDB30 *self)
{
    WorkDB30 *work;

    work = self->work;
    if ((work->flags & 0x1000) != 0) {
        func_088D3F00(&work->layout, work->handles, D_08ADA1CA);
        func_088D44EC(self->work->handles, self->work->texts, D_08ADA1EA);
    }
}
