#include "menu/auto_region4.h"

/* ScreenB01CF8: resets, then takes its 0x1F000-byte block from the heap. */
void func_08AF7760(ScreenB01CF8X *self)
{
    func_08AF7794(self);
    func_08AF7BFC(self, 0x1F000);
}
