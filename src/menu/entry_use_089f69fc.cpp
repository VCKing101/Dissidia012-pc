#include "menu/agent29_types.h"

/* Screen0B6A0: uses the current entry: plays its effect on the common
   frames, motion 0x91 on its lamp and, when func_089F32D4 counted any,
   motion 0 on the page model (handle 0x89); then refreshes the slots and
   lamps and raises page flag 0x4000. */
extern "C" {
int func_089F32D4(Screen0B6A0 *self, int arg1, int arg2);
void func_08A0118C(Screen0B6A0 *self, Entry84 *entry, int frame, int sound, int text, int arg5,
                   int arg6);
void func_089F24D8(Screen0B6A0 *self);
void func_089F652C(Screen0B6A0 *self);
void func_089F6AF4(Screen0B6A0 *self);
}

/* The first frame of an entry's row of ten handles (from handle 0x25). */
struct EntryRows29 {
    static MenuModel *Frame(Work0B6A0 *work, int row) { return work->handles[0x25 + row]; }
};

extern "C" void func_089F69FC(Screen0B6A0 *self)
{
    int current = func_089F2FF0(self);
    int counted = func_089F32D4(self, 1, 1);
    int motion = 0x91;

    func_08A0118C(self, &self->work->entries[current], 0x80, 0x2B, 0x30, 1, 0);
    func_088D9F5C(EntryRows29::Frame(self->work, current * 10), motion, 1);
    if (counted) {
        func_088D9F5C(self->work->handles[0x89], 0, 1);
    }
    func_089F24D8(self);
    func_089F652C(self);
    self->work->flags |= 0x4000;
    func_089F6AF4(self);
}
