#include "menu/auto_region3.h"

extern const short D_08AA8220[];

/* ScreenA7ED0: plays script D_08AA8220 over its 0x120 handles. */
void func_08AA46AC(ScreenA7ED0 *self)
{
    func_088D3C24(self->work->layout, self->work->handles, D_08AA8220, 0x120);
}
