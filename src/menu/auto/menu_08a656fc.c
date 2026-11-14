#include "menu/auto_region2.h"

/* Asks the save screen a yes/no question (message 0x62), returns 1. */
int func_08A656FC(void)
{
    func_08A4E2E4(D_08A4FFF4, 0, func_08A63890, func_08A63830, func_08A63830, 0x62, -1, -1, -1);
    return 1;
}
