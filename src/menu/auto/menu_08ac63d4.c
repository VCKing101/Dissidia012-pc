#include "menu/auto_region3.h"

/* ScreenC47C4: steps its page; when it is done, sets field_A04 and runs func_08AC64A8. */
void func_08AC63D4(ScreenC47C4 *self)
{
    int done;

    func_08AC593C(self);
    done = func_08AC8A00(self->work->field_980);
    if (done != 0) {
        self->work->field_A04 = 1;
        func_08AC64A8(self);
    }
}
