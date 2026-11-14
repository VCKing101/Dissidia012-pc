#include "menu/agent29_types.h"

/* Screen0B6A0: waits on the current entry's lamp (row frame) and the
   page model (handle 0x89): 0 while the lamp is in state 0x3D, 1 until
   both are done, func_089F66E0 agrees and the counter at +0x3210 has run
   out; then shows page 0x80, moves on and gives 2. */
extern "C" {
int func_089F66E0(Screen0B6A0 *self);
int func_089F2758(Screen0B6A0 *self, void *counter);
void func_089F6754(Screen0B6A0 *self);
}

/* The first frame of an entry's row of ten handles (from handle 0x25). */
struct EntryRows29 {
    static MenuModel *Frame(Work0B6A0 *work, int row) { return work->handles[0x25 + row]; }
    static bool Done(MenuModel *model) { return (model->field_20 & 0x20) != 0; }
};

extern "C" int func_089F6C20(Screen0B6A0 *self)
{
    int result = 0;
    int current = func_089F2FF0(self);

    if (EntryRows29::Frame(self->work, current * 10)->field_0C != 0x3D) {
        if (EntryRows29::Done(EntryRows29::Frame(self->work, current * 10))
            && (self->work->handles[0x89]->field_0C != 0 || EntryRows29::Done(self->work->handles[0x89]))
            && func_089F66E0(self) != 0
            && func_089F2758(self, &WORK_BYTE(self->work, 0x3210)) == 0) {
            func_089F5990(self, 0x80);
            func_089F6754(self);
            result = 2;
        } else {
            result = 1;
        }
    }
    return result;
}
