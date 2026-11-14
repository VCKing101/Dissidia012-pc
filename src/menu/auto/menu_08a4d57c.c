#include "menu/auto_region2.h"

/* D_08A4FFF4: field_44 = 0, then func_08A4C2B8(flag). */
void func_08A4D57C(unsigned char flag)
{
    D_08A4FFF4->field_44 = 0;
    func_08A4C2B8(D_08A4FFF4, flag);
}
