#include "battle/auto_region5.h"

/* Releases D_08B125C8 and the block at D_09B6C2B0. */
void func_08B12550(void)
{
    if (D_08B125C8 != 0) {
        func_088DAB44(D_08B125C8);
        D_08B125C8 = 0;
    }
    func_0887AD1C(D_09B6C2B0);
}
