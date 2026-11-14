#include "menu/auto_region3.h"

/* Frees a block from the heap of the screen in D_08AC7500. */
void func_08AC888C(void *block)
{
    func_0885C538(((ScreenC47C4 *)D_08AC7500)->heap, block, 0);
}
