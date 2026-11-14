#include "menu/auto_region3.h"

/* ScreenDB30: reloads; runs func_08ACEEDC when func_08ACE74C finds nothing. */
void func_08ACF008(ScreenDB30 *self)
{
    int found;

    func_08ACEF7C();
    found = func_08ACE74C(self, 0, 0);
    if (found == 0) {
        func_08ACEEDC(self);
    }
    func_08ACEFA4(self);
}
