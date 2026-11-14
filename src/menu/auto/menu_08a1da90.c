#include "menu/auto_region1.h"

/* Menu root: 1 when func_08A1D8E4 reports 0. */
unsigned char func_08A1DA90(MenuRoot *self)
{
    int ready;

    ready = func_08A1D8E4(self);
    return ready == 0;
}
