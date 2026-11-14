#include "battle/auto_region5.h"

/* Formats D_089D6AC8's name into a buffer and, when it matches
 * D_08B542BC, runs func_08B5D630 on D_08B54330's field_1054. */
void func_08B53830(void)
{
    char buffer[8];

    func_089346D0(buffer, 5, D_08B542B8, GLOBAL_9D6AC8->field_710->field_08);
    if (func_08935C14(buffer, D_08B542BC) != 0) {
        func_08B5D630(D_08B54330->field_1054);
    }
}
