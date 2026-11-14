#include "menu/auto_region1.h"

/* Screen392E8: plays motion 0x35 on handle 7 (direction < 0) or 0x33 on
 * handle 8. */
void func_08A39504(Screen392E8 *self, int direction)
{
    MenuModel **handles;

    handles = self->work->handles;
    if (direction < 0) {
        func_088D9F5C(handles[7], 0x35, 1);
    } else {
        func_088D9F5C(handles[8], 0x33, 1);
    }
}
