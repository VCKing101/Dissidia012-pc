#include "menu/auto_region3.h"

extern const short D_08AAFEA0[];

/* ScreenAB7CFC: plays script D_08AAFEA0 over its 0x120 handles. */
void func_08AA9604(ScreenAB7CFC *self)
{
    func_088D3C24(self->work->layout, self->work->handles, D_08AAFEA0, 0x120);
}
