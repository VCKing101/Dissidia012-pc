#include "menu/agent29_types.h"

/* ScreenE75C4: plays the select motions of the current row (+0xA88) and,
   when the entry of its current part (+0xA5C) is enabled, motion 0x11 on
   that part's mark. */
void func_089E25BC(ScreenE75C4 *self)
{
    Row7518 *row = &D_089E7518[WORK_INT(self->work, 0xA88)];
    unsigned short label = row->label;
    RowPart *parts = row->parts;
    RowPart *part;

    func_088D9F5C(self->work->handles[row->frame], 1, 1);
    func_088D9F5C(self->work->handles[label], 0xE, 1);
    part = &parts[WORK_INT(self->work, 0xA5C)];
    if (D_089E7488[part->entry].enabled != 0) {
        func_088D9F5C(self->work->handles[part->mark], 0x11, 1);
    }
}
