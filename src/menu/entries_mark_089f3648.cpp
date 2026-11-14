#include "menu/agent29_types.h"

/* Screen0B6A0: flashes (motion 0x31) every one of the first `count`
   entries with a flag in `mask`, marks them (0x10), keeps the first in
   +0x3454 and raises page flag 0x100. */
extern "C" void func_089F3790(Screen0B6A0 *self);

/* The first frame of an entry's row of ten handles (from handle 0x25). */
struct EntryRows29 {
    static MenuModel *Frame(Work0B6A0 *work, int row) { return work->handles[0x25 + row]; }
};

extern "C" {

void func_089F3648(Screen0B6A0 *self, int count, unsigned int mask)
{
    int i;

    WORK_INT(self->work, 0x3454) = -1;
    for (i = 0; i < count; i++) {
        if ((self->work->entries[i].flags & mask) != 0) {
            func_088D9F5C(EntryRows29::Frame(self->work, i * 10), 0x31, 1);
            self->work->entries[i].flags |= 0x10;
            if (WORK_INT(self->work, 0x3454) == -1) {
                WORK_INT(self->work, 0x3454) = i;
            }
        }
    }
    self->work->flags |= 0x100;
    func_089F3790(self);
}
}
