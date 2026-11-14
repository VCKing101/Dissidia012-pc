#include "menu/auto_region2.h"

/* D_08A4FFF4: field_44 = value, then starts its task. */
void func_08A4D5A8(int value)
{
    D_08A4FFF4->field_44 = value;
    func_08A4C304(D_08A4FFF4);
}
