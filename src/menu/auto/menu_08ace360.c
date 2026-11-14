#include "menu/auto_region3.h"

/* ScreenDB30: reloads; 4 if func_08ACFFCC finds something, else 2. */
int func_08ACE360(ScreenDB30 *self)
{
    int found;
    int result;

    func_08ACEF7C();
    func_08ACE27C(self, 0);
    func_08ACEFA4(self);
    result = 2;
    found = func_08ACFFCC(self);
    if (found != 0) {
        result = 4;
    }
    func_08ACADD4(self);
    func_08AD1514(self, 0);
    return result;
}
