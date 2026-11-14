#include "menu/auto_region4.h"

void func_08B3D720(void)
{
    ScreenB41CC0 *screen;

    screen = D_08B41CC0;
    D_08B41CC0->field_0C = D_08B41CC0->field_0C + 1;
    func_08B35CF0(screen, 1);
    func_08B41F44(D_08B42088, 8);
}
