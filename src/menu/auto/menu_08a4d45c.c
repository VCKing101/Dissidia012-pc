#include "menu/auto_region2.h"

/* Opens a system dialog (D_089D7E88 field_184D set), done callback func_08A4D650. */
void func_08A4D45C(unsigned int arg1, unsigned int arg2, unsigned char arg3)
{
    D_089D7E88->field_184D = 1;
    func_088E9958(0);
    func_088E98B4(arg2, arg3, 0);
    func_088E991C(0, func_08A4D650);
}
