#include "battle/auto_region5.h"

/* Hands items 0x1A and 0x1B of what func_08899300 returns to
 * func_08B5DC80. */
void func_08B5D630(void *self)
{
    void *list;
    void *item;

    list = func_08899300();
    item = func_088985F0(list, 0x1A);
    func_08B5DC80(self, item, 1);
    item = func_088985F0(list, 0x1B);
    func_08B5DC80(self, item, 1);
}
