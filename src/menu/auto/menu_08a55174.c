#include "menu/auto_region2.h"

/* Item: func_08A55070, then hides handles 3 and 4 and texts 3, 6, 9 that exist. */
void func_08A55174(Item *self)
{
    func_08A55070(self);
    if (self->handles[3] != 0) {
        func_088D9CFC(self->handles[3]);
    }
    if (self->handles[4] != 0) {
        func_088D9CFC(self->handles[4]);
    }
    if (self->texts[3] != 0) {
        func_0887B944(self->texts[3]);
    }
    if (self->texts[6] != 0) {
        func_0887B944(self->texts[6]);
    }
    if (self->texts[9] != 0) {
        func_0887B944(self->texts[9]);
    }
}
