#include "menu/auto_region1.h"

/* Menu root: opens notice kind through D_089D727C and sets bit 0x8000 of
 * field_D80 when it opened. */
void func_08A20590(MenuRoot *self, int kind)
{
    int opened;

    opened = func_088B1194(D_089D727C, kind, func_08A2044C, 0, 0xA0);
    if (opened != 0) {
        self->work->field_D80 = self->work->field_D80 | 0x8000;
    }
}
