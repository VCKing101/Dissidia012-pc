#include "menu/agent29_types.h"

/* Screen0B6A0: plays the entry page's script, then motion 4 on the mark
   of every entry with flag 0x100000, with sound 0x22 when any did. */
extern const short D_08A094DE[];

void func_089F6784(Screen0B6A0 *self)
{
    int played = 0;
    int i;

    func_088D3B88(&self->work->layout, self->work->handles, D_08A094DE, 0, 0x180);
    for (i = 0; i < self->work->field_2731; i++) {
        if (self->work->entries[i].flags & 0x100000) {
            func_088D9F5C(self->work->handles[0x8A + i], 4, 1);
            played = 1;
        }
    }
    if (played) {
        func_088D4BF4(0x22);
    }
}
