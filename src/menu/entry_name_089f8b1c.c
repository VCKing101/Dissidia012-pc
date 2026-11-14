#include "menu/agent29_types.h"

/* Screen0B6A0: once (page flag 1 of +0x3238), plays the entry list's
   script, remakes the entry name texts (0x23 and its shadow 0x24) for
   the entry +0x279E, plays their script, sets step 0xD and moves on. */
extern const unsigned short D_08A0A1DA[];
extern const unsigned short D_08A0A1FA[];
extern void *func_089FBB90(Screen0B6A0 *self, unsigned short id, unsigned char with_count);
extern void func_089F8C24(Screen0B6A0 *self);

void func_089F8B1C(Screen0B6A0 *self)
{
    if (WORK_INT(self->work, 0x3238) & 1) {
        return;
    }
    func_088D3CF0(&self->work->layout, self->work->handles, D_08A0A1DA);
    if (self->work->texts[0x23] != 0) {
        func_0887BA5C(self->work->texts[0x23]);
        self->work->texts[0x23] = 0;
    }
    if (self->work->texts[0x24] != 0) {
        func_0887BA5C(self->work->texts[0x24]);
        self->work->texts[0x24] = 0;
    }
    self->work->texts[0x23] = func_089FBB90(self, WORK_U16(self->work, 0x279E), 0);
    self->work->texts[0x24] = func_0887E534(self->work->texts[0x23]);
    func_088D428C(self->work->handles, self->work->texts, D_08A0A1FA, 1);
    WORK_INT(self->work, 0x323C) = 0xD;
    WORK_INT(self->work, 0x3238) |= 1;
    func_089F8C24(self);
}
