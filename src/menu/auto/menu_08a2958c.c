#include "menu/auto_region1.h"

extern int D_09B6D9F8[];

/* Drops D_08A29BF0 and clears the five words at D_09B6D9F8. */
void func_08A2958C(void)
{
    int i;

    if (D_08A29BF0 != 0) {
        D_08A29BF0 = 0;
        for (i = 0; i < 5; i++) {
            D_09B6D9F8[i] = 0;
        }
    }
}
