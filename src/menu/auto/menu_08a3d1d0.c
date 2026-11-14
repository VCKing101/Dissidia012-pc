#include "menu/auto_region2.h"

/* Screen40248: plays the back (0x2B on handle 0x49) or forward (0x2F on handle 0x4A) motion. */
void func_08A3D1D0(Screen40248 *self, int step)
{
    MenuModel **handles;

    handles = self->work->handles;
    if (step < 0) {
        func_088D9F5C(handles[0x49], 0x2B, 1);
    } else {
        func_088D9F5C(handles[0x4A], 0x2F, 1);
    }
}
