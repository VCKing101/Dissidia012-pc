#include "menu/auto_region3.h"

/* ScreenDB30: func_08AF4E1C on D_08B01CF8, then refreshes. */
void func_08ACE3D0(ScreenDB30 *self)
{
    func_08AF4E1C(D_08B01CF8);
    func_08AD6698(self);
    func_08ACADD4(self);
    func_08AD1514(self, 0);
}
