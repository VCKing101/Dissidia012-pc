#include "menu/agent29_types.h"

/* Screen0B6A0: counts one more pull (+0x27A0, at most 999) and goes on to
   the result (func_089F2E7C in mode 1 of D_09B6DA40, else
   func_089F2F30). */
extern int D_09B6DA40;
extern void func_089F2E7C(Screen0B6A0 *self);
extern void func_089F2F30(Screen0B6A0 *self);
extern int func_088F29F4(void);

void func_089F2F58(Screen0B6A0 *self)
{
    WORK_SHORT(self->work, 0x27A0)++;
    if (WORK_SHORT(self->work, 0x27A0) >= 1000) {
        WORK_SHORT(self->work, 0x27A0) = 999;
    }
    if (D_09B6DA40 == 1) {
        func_089F2E7C(self);
    } else if (func_088F29F4() != 0) {
        func_089F2F30(self);
    } else {
        func_089F2F30(self);
    }
}
