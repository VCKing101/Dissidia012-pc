#include "menu/auto_region1.h"

extern int D_08A3CCF0;
extern int D_08A3CD00;
extern int D_09B6DA98[];

/* Clears D_08A3CCF0, D_08A3CD00 and the nine words at D_09B6DA98, when
 * D_08A3CCF0 is set. */
void func_08A3BF2C(void)
{
    int i;

    if (D_08A3CCF0 != 0) {
        D_08A3CCF0 = 0;
        D_08A3CD00 = 0;
        for (i = 0; i < 9; i++) {
            D_09B6DA98[i] = 0;
        }
    }
}
