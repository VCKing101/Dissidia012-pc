#include "menu/auto_region3.h"

extern const short D_08B0000E[];
extern const unsigned short D_08B009FC[];

/* ScreenB01CF8: resets a layout and its handles and plays the opening scripts. */
void func_08AF3698(ScreenB01CF8 *self, MenuLayout *layout, MenuModel **handles)
{
    func_088D38F0(layout);
    func_088D3B5C(handles, 0x120);
    func_088D3910(5, layout, D_08B0000E, 0, 1);
    func_08AF37A4(self, layout, handles, 5, D_08B009FC);
}
