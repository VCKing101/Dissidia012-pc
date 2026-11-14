#include "menu/auto_region1.h"

/* Menu root: opens the notice pending in field_25B84 through D_089D727C
 * (kinds 0 and 2 with func_088B1354, others with func_088B1194), setting
 * flag 0x80 or 0x400 when it opened, then clears the pending kind. */
void func_08A20474(MenuRoot *self)
{
    int kind;
    int opened;

    kind = self->work->field_25B84;
    if (kind != -1) {
        if (kind == 0) {
            opened = func_088B1354(D_089D727C, 0, func_08A2044C, 0, 0xA0);
            if (opened != 0) {
                self->work->flags = self->work->flags | 0x80;
            }
        } else if (kind == 2) {
            opened = func_088B1354(D_089D727C, 2, func_08A20688, 0, 0xA0);
            if (opened != 0) {
                self->work->flags = self->work->flags | 0x400;
            }
        } else {
            opened = func_088B1194(D_089D727C, kind, func_08A2044C, 0, 0xA0);
            if (opened != 0) {
                self->work->flags = self->work->flags | 0x80;
            }
        }
        func_08A20434(self);
    }
}
