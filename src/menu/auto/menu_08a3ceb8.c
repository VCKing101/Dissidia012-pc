#include "menu/auto_region2.h"

/* Screen40248: plays the back (0x35 on handle 6) or forward (0x33 on handle 7) motion. */
void func_08A3CEB8(Screen40248 *self, int step)
{
    MenuModel **handles;

    handles = self->work->handles;
    if (step < 0) {
        func_088D9F5C(handles[6], 0x35, 1);
    } else {
        func_088D9F5C(handles[7], 0x33, 1);
    }
}
