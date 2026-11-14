#include "menu/agent29_types.h"

/* ScreenE75C4: plays the script of every row that has one, then motion
   0x13 on each part of every row whose entry is not enabled. */

void func_089E20AC(ScreenE75C4 *self)
{
    int i;
    RowPart *part;

    for (i = 0; i < 7; i++) {
        if (D_089E7518[i].label != 0xFFFF) {
            func_088D3CF0(&self->work->layout, self->work->handles, D_089E7518[i].script);
            func_088D3EA0(&self->work->layout, self->work->handles, D_089E7518[i].script);
        }
    }
    for (i = 0; i < 7; i++) {
        part = D_089E7518[i].parts;
        if (part != 0) {
            while (part->handle != 0xFFFF) {
                if (D_089E7488[part->entry].enabled == 0) {
                    func_088D9F5C(self->work->handles[part->handle], 0x13, 1);
                }
                part++;
            }
        }
    }
}
