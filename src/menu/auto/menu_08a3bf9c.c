#include "menu/auto_region1.h"

extern int D_08A3CCF4;
extern int D_08A3CCF8;
extern int D_08A3CCFC;

/* Clears D_08A3CCF4, D_08A3CCF8 and D_08A3CCFC, when D_08A3CCF4 is set. */
void func_08A3BF9C(void)
{
    if (D_08A3CCF4 != 0) {
        D_08A3CCF4 = 0;
        D_08A3CCF8 = 0;
        D_08A3CCFC = 0;
    }
}
