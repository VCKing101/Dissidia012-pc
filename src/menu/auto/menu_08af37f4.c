#include "menu/auto_region3.h"

extern const short D_08B0002A[];

/* Plays the closing script D_08B0002A over 0x120 handles. */
void func_08AF37F4(ScreenB01CF8 *self, MenuLayout *layout, MenuModel **handles)
{
    func_088D3C24(layout, handles, D_08B0002A, 0x120);
}
