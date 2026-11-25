#include "menu/agent36_types.h"

/* Screen28560: copies the pose of the current kind's part (handle 0x5C)
 * onto handle 0x59. */
void func_08A250FC(Screen28560x *self, Model36 **handles)
{
    float matrix[16];
    int color;
    int kind = self->work->kind;

    if (kind == -1) {
        return;
    }
    {
        const unsigned short *ps = D_08A28AE8[kind].parts;
        int part = self->work->part;
        Model36 *h = handles[0x5C];
        func_088DA0A8(h, ps[part], matrix, &color);
    }
    func_088D9B84(handles[0x59], matrix);
    func_088D9AF4(handles[0x59], &color);
}
