#include "menu/auto_region1.h"

/* Screen0B6A0: when the current entry has flag 0x100, plays motion 0x83 on
 * handle 0x7F, hides the entry's nine handles, redraws it and sets flag 4. */
void func_089F7420(Screen0B6A0 *self)
{
    int index;

    index = func_089F2FF0(self);
    if ((self->work->entries[index].flags & 0x100) != 0) {
        func_088D9F5C(self->work->handles[0x7F], 0x83, 1);
        func_089F5990(self, index * 10 + 0x26);
        func_08A0118C(self, &self->work->entries[index], 0x80, 0x2B, 0x30, 0, 0);
        self->work->flags = self->work->flags | 4;
        func_089F74E4(self);
    }
}
