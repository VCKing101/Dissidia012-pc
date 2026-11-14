#include "battle/auto_region4.h"

/* Clears D_089D4928 and frees the object at D_089D492C. */
void func_08AD28C4(void)
{
    D_089D4928 = 0;
    func_08842138(D_089D492C);
    D_089D492C = 0;
}
