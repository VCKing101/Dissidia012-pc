/* libgcc2: __cmpdi2, signed 64-bit three-way compare. */
#include "main/sdk_gcc1_types.h"

SItype func_0899B2C8(DItype a, DItype b)
{
    DWunion au, bu;

    au.ll = a, bu.ll = b;

    if (au.s.high < bu.s.high)
        return 0;
    else if (au.s.high > bu.s.high)
        return 2;
    if ((USItype)au.s.low < (USItype)bu.s.low)
        return 0;
    else if ((USItype)au.s.low > (USItype)bu.s.low)
        return 2;
    return 1;
}
