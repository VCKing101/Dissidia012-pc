#include "menu/auto_region1.h"

/* Screen35550: plays motion 0x35 on handle 6 (direction < 0) or 0x33 on
 * handle 7. */
void func_08A2EB80(Screen35550 *self, int direction)
{
    MenuModel **handles;

    handles = self->work->handles;
    if (direction < 0) {
        func_088D9F5C(handles[6], 0x35, 1);
    } else {
        func_088D9F5C(handles[7], 0x33, 1);
    }
}
