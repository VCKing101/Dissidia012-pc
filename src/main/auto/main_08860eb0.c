#include "main/auto_region2.h"

/* Switches back to the clock mode before the last switch. */
void func_08860EB0(void)
{
    D_089D5E80 = D_089D5E84;
    if (D_089D5E84 == 0) {
        func_08860E78();
    } else {
        func_08860E3C();
    }
}
