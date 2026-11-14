#include "menu/agent29_types.h"

/* ScreenE75C4 per-frame update: polls the pad while the screen is not
   locked (+0xF88), then runs the page step selected by flags 4 and 8.
   Returns 1 while the page is idle. */
extern "C" {

int func_089E1CA4(ScreenE75C4 *self)
{
    int result;
    int refresh;

    if (WORK_BYTE(self->work, 0xF88) == 0) {
        int pressed = func_088F0A78(1);
        if (pressed != 0) {
            if (func_088F1474() == 0) {
                func_089E3094(self, 0);
            } else {
                func_089E3094(self, 1);
            }
        }
        if (pressed != 0) {
            func_089E5C18(self);
        }
    }
    result = 1;
    refresh = 1;
    if (Flags29::Has(self->work->flags, 4)) {
        if ((self->work->flags & 8) != 0) {
            refresh = 0;
            result = 0;
            if (func_089E5E7C(self) != 0) {
                self->work->flags &= ~8;
            } else {
                func_089E2E04(self);
            }
        } else if (func_089E2CB0(self) != 0) {
            refresh = 0;
            result = 0;
        }
    } else {
        result = 0;
    }
    if (refresh) {
        func_089E2698(self);
    }
    return result;
}

}
