#include "menu/auto_region3.h"

/* ScreenC47C4: refreshes its rows and texts. */
void func_08AC58D0(ScreenC47C4 *self)
{
    char buffer[128];

    func_08AC4990(self);
    func_08AC4AA4(self, 0x40, buffer);
    func_08AC4C00(self, 0x40, buffer);
    func_08AC53AC(self);
    func_08AC4E44(self, 0x40, buffer);
    func_08AC6920(self);
    func_08AC5708(self);
}
