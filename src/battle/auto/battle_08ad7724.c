#include "battle/auto_region4.h"

/* Clears D_089D4930 and frees the object at D_089D4934. */
void func_08AD7724(void)
{
    D_089D4930 = 0;
    func_08842138(D_089D4934);
    D_089D4934 = 0;
}
