#include "menu/agent36_types.h"

/* Menu root: plays the pose motions on handles 0x28/0x29 for the current
 * pose kind (if it has a model), then copies the pose. */

void func_08A1FF88(MenuRoot36 *self, void **handles)
{
    int kind = self->work->pose_kind;
    int motion;
    int second;

    if (kind == -1) {
        return;
    }
    if (D_08A23FB0[kind].model != -1) {
        motion = 0x24;
        second = 0;
        if (self->work->pose == 0) {
            motion = 0x25;
        } else {
            second = 1;
        }
        func_088D9F5C(handles[0x28], motion, 1);
        func_088D9F5C(handles[0x29], second, 1);
        func_08A2005C(self, handles);
    }
}
