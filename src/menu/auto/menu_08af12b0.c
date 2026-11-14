#include "menu/auto_region3.h"

/* Frees a panel block from the heap of the screen in D_08B01CF8. */
void func_08AF12B0(void *block)
{
    func_0885C538(((ScreenB01CF8 *)D_08B01CF8)->heap, block, 0);
}
