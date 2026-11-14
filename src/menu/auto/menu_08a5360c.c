#include "menu/auto_region2.h"

/* Widget54438: hides handles 8..12. */
void func_08A5360C(Widget54438 *self)
{
    int i;

    for (i = 8; i < 13; i++) {
        func_088D9CFC(self->handles[i]);
    }
}
