#include "menu/auto_region4.h"

/* Opens ScreenB41CC0 on kind and starts its task. */
void func_08B3C900(int kind, int arg1, int arg2, int arg3)
{
    func_08B40E60(D_08B41CC0, kind, arg1, arg2, arg3);
    func_088650A4((MenuTask *)&D_08B41CC0->task);
}
