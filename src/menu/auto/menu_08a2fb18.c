#include "menu/auto_region1.h"

/* Screen35550: plays motion 0x2B on handle 0x20 (direction < 0) or 0x2F on
 * handle 0x21. */
void func_08A2FB18(Screen35550 *self, int direction)
{
    MenuModel **handles;

    handles = self->work->handles;
    if (direction < 0) {
        func_088D9F5C(handles[0x20], 0x2B, 1);
    } else {
        func_088D9F5C(handles[0x21], 0x2F, 1);
    }
}
