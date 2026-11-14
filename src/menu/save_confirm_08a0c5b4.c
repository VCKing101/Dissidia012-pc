#include "menu/agent29_types.h"

/* Screen11510: the save confirm step: while the root's prompt is open
   (page flags 2 and 4), waits for it to close (and, for 2, for the root),
   then returns to step 3; otherwise asks the root for the answer and
   opens the overwrite (1) or new-save (-1) prompt. The root's prompt is
   drawn every frame. */
extern int func_08A1ED38(MenuRoot *root, MenuLayout *layout, MenuModel **handles, void **texts);
extern int func_08A1DA90(MenuRoot *root);
extern int func_08A1D9E4(MenuRoot *root);
extern void func_08A1DA4C(MenuRoot *root);
extern void func_08A1EC00(MenuRoot *root, MenuLayout *layout, MenuModel **handles, void **texts);
extern void func_08A22948(MenuRoot *root, MenuLayout *layout, MenuModel **handles, void **texts);
extern void func_08A0C330(Screen11510 *self);

void func_08A0C5B4(Screen11510 *self)
{
    int answer;

    if (WORK_INT(self->work, 0xFD8) & 2) {
        if (func_08A1ED38(D_08A23FF0, &self->work->layout, self->work->handles, self->work->texts) != 0
            && func_08A1DA90(D_08A23FF0) != 0) {
            WORK_INT(self->work, 0xFD8) &= ~2;
            self->work->state = 3;
            func_08A0C330(self);
        }
    } else if (WORK_INT(self->work, 0xFD8) & 4) {
        if (func_08A1ED38(D_08A23FF0, &self->work->layout, self->work->handles, self->work->texts) != 0) {
            WORK_INT(self->work, 0xFD8) &= ~4;
            self->work->state = 3;
            func_08A0C330(self);
        }
    } else {
        answer = func_08A1D9E4(D_08A23FF0);
        if (answer == 1) {
            func_08A1DA4C(D_08A23FF0);
            func_08A1EC00(D_08A23FF0, &self->work->layout, self->work->handles, self->work->texts);
            WORK_INT(self->work, 0xFD8) |= 2;
        } else if (answer == -1) {
            func_08A1EC00(D_08A23FF0, &self->work->layout, self->work->handles, self->work->texts);
            WORK_INT(self->work, 0xFD8) |= 4;
        }
    }
    func_08A22948(D_08A23FF0, &self->work->layout, self->work->handles, self->work->texts);
}
