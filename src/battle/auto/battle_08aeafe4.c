#include "battle/auto_region4.h"

/* Releases the model at D_08AEB9BC, if loaded. */
void func_08AEAFE4(void)
{
    if (D_08AEB9BC != 0) {
        func_088D0A1C(D_089D7830, D_08AEB9BC);
        D_08AEB9BC = 0;
    }
}
