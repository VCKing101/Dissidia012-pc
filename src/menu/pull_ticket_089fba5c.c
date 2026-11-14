#include "menu/agent29_types.h"

/* Screen0B6A0: when the save has a free ticket, opens the ticket prompt
   (kind 6) and raises flag 0x20 of +0x38A4; returns 1 when there is none. */
extern int func_088F4228(void *table);
extern void func_08A05F60(Screen0B6A0 *self, int kind, void *layout, void *handles, void *texts,
                          int arg);

int func_089FBA5C(Screen0B6A0 *self)
{
    int none = 0;

    if (func_088F4228((char *)D_089D712C + 0x1E208) != 0) {
        func_088D4BF4(1);
        func_08A05F60(self, 6, &self->work->layout, self->work->handles, self->work->texts, -1);
        self->work->field_38A4 |= 0x20;
    } else {
        none = 1;
    }
    return none;
}
