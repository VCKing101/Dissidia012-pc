#include "menu/auto_region4.h"

/* Obj29F3C step: once D_08B59230 is ready, refreshes (all of it when its
 * state is 3) and moves to state 3. */
int func_08B29F3C(Obj29F3C *self)
{
    int ready;

    func_08B2A970(self);
    ready = func_08B58F8C(D_08B59230);
    if (ready != 0) {
        if (((Obj59230 *)D_08B59230)->field_00->field_0C == 3) {
            func_08B2A82C(self);
            func_08B2AA04(self);
            func_08B2ABF8(self);
        }
        func_08B2B0C4(self);
        self->work->state = 3;
    }
    return 0;
}
