#include "menu/agent29_types.h"

/* Screen0B6A0: when the current record (+0x328C) can be pulled, opens the
   pull prompt (kind 4) and raises flag 4 of +0x38A4; otherwise plays the
   refusal sound and returns 1. */
extern int func_088F40AC(void *table, int record);
extern void func_08A05F60(Screen0B6A0 *self, int kind, void *layout, void *handles, void *texts,
                          int arg);

int func_089FBAEC(Screen0B6A0 *self)
{
    int none = 0;

    if (func_088F40AC((char *)D_089D712C + 0x1E208, WORK_INT(self->work, 0x328C)) != 0) {
        func_088D4BF4(1);
        func_08A05F60(self, 4, &self->work->layout, self->work->handles, self->work->texts, -1);
        self->work->field_38A4 |= 4;
    } else {
        func_088D4BF4(0x11);
        none = 1;
    }
    return none;
}
