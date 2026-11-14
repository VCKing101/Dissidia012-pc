#include "menu/auto_region3.h"

extern const short D_08AF1068[];

/* Plays script D_08AF1068 over 0x120 handles. */
void func_08AF08A4(MenuLayout *layout, MenuModel **handles)
{
    func_088D3B88(layout, handles, D_08AF1068, 0, 0x120);
}
