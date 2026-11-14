#include "menu/auto_region3.h"

/* ScreenDB30: looks func_08ACF738's value up; on a hit runs func_08ACE24C. */
int func_08ACF7C4(ScreenDB30 *self)
{
    int value;
    int result;

    value = func_08ACF738(self);
    result = func_08ACF81C(self, value, 0);
    if (result != -1) {
        func_08ACE24C(self);
    }
    return result;
}
