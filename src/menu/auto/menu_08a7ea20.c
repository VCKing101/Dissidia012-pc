#include "menu/auto_region3.h"

/* StepWait: opens dialog 0x80410B03; func_08A7EA9C sets done when it closes. */
void func_08A7EA20(StepWait *self)
{
    func_088B1194(D_089D727C, 0x80410B03, func_08A7EA9C, (int)self, 0xA0);
}
