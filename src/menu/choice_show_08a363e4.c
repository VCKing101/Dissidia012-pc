#include "menu/agent36_types.h"

/* Choice screen: marks the chosen of the two options (handles 9, 10), plays
 * the choice script and the motions of the chosen option. */

extern ScriptCmd36 D_08A38F1C[];

void func_08A363E4(ClockScreen36 *self, int choice)
{
    int motion = 0x13;
    int i;

    if (self->work->field_DB4 != 0) {
        motion = 0x14;
    }
    for (i = 0; i < 2; i++) {
        func_088D9F5C(self->work->handles[9 + i], motion, 1);
    }
    func_088D9F5C(self->work->handles[9 + choice], 0xC, 1);
    self->work->script = D_08A38F1C;
    func_088D3CF0(self->work->layout, self->work->handles, self->work->script);
    func_088D9CFC(self->work->handles[8]);
    switch (choice) {
    case 0:
        func_088D9F5C(self->work->handles[6], 0x10, 1);
        if (self->work->field_DB5 != 0) {
            func_088D9F5C(self->work->handles[8], 7, 1);
        }
        break;
    case 1:
        func_088D9F5C(self->work->handles[7], 0x12, 1);
        break;
    }
}
