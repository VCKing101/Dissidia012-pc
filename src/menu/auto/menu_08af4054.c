#include "menu/auto_region3.h"

/* ScreenB01CF8: 1 unless one of four checks reports something. */
int func_08AF4054(ScreenB01CF8 *self)
{
    int found;
    int result;

    result = 1;
    found = func_08AF96B0();
    if ((((found != 0) || (found = func_08AF9F1C(self), found != 0)) ||
         (found = func_08AF9F94(self), found != 0)) ||
        (found = func_08AF9FB4(self), found != 0)) {
        result = 0;
    }
    return result;
}
